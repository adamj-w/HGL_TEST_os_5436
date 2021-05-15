#include <libsystem/Format.h>
#include <libsystem/Formattable.h>

#include <libc/string.h>

namespace hegel {

ErrorOr<size_t> format(Stream& stream, const char* fmt)
{
    return stream.write(fmt, strlen(fmt));
}

ErrorOr<size_t> format(Stream& stream, const char* string, FormatInfo& info)
{
    __unused(info);

    return stream.write(string, strlen(string));
}

ErrorOr<size_t> format(Stream& stream, const void* ptr, FormatInfo& info)
{
    return format(stream, reinterpret_cast<uintptr_t>(ptr), info);
}

static const char* get_prefix(int base)
{
    switch(base) {
    case 2:
        return "0b";
    case 8:
        return "0";
    case 16:
        return "0x";
    default:
        return "";
    }
}

static const char *basechar_maj = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static int uint_to_str(unsigned int value, char* str, unsigned int base) {
    assert(str);
    assert(base < 36);

    str[0] = '\0';
    if(value == 0) {
        strapd(str, '0');
        return 1;
    }

    int written = 0;
    while(value != 0) {
        strapd(str, basechar_maj[value % base]);
        value /= base;

        written++;
    }

    strrvs(str);
    return written;
}

ErrorOr<size_t> format(Stream& stream, unsigned int value, FormatInfo& info)
{
    char buffer[32];
    uint_to_str(value, buffer, info.base);

    if(info.prefix) {
        return format(stream, "{}{}", get_prefix(info.base), buffer);
    } else {
        return stream.write(reinterpret_cast<void*>(buffer), strlen(buffer));
    }
}

ErrorOr<size_t> format(Stream& stream, Formattable& value, FormatInfo& info)
{
    return value.format(stream, info);
}

ErrorOr<size_t> format(Stream& stream, Formattable* value, FormatInfo& info)
{
    return value->format(stream, info);
}

}