#pragma once

#include <libruntime/ErrorOr.h>
#include <libsystem/Stream.h>

namespace hegel {

class Formattable;

struct FormatInfo
{
    bool prefix = false;
    int base = 10;
};

ErrorOr<size_t> format(Stream& stream, const char* fmt);

template<typename First, typename... Args>
ErrorOr<size_t> format(Stream& stream, const char* fmt, First first, Args... args)
{
    size_t written = 0;

    for(size_t i = 0; fmt[i]; i++) {
        if(fmt[i] == '{') {
            FormatInfo info;

            for(; fmt[i] != '}'; i++) {
                switch(fmt[i]) {
                case '#':
                    info.prefix = true;
                    break;
                case 'x':
                    info.base = 16;
                    break;
                case 'd':
                    info.base = 10;
                    break;
                case 'o':
                    info.base = 8;
                    break;
                case 'b':
                    info.base = 2;
                    break;
                default:
                    break;
                }
            }

            ErrorOr<size_t> res_format = format(stream, first, info);

            written += res_format.value();

            if(res_format != Error::SUCCEED) {
                return ErrorOr<size_t>(res_format.error(), written);
            }

            ErrorOr<size_t> res_print = format(stream, &fmt[i+1], args...);

            written += res_print.value();

            if(res_print != Error::SUCCEED) {
                return ErrorOr<size_t>(res_print.error(), written);
            } else {
                return ErrorOr<size_t>(written);
            }
        } else {
            auto error = stream.write_byte(fmt[i]);

            if(error != Error::SUCCEED) {
                return ErrorOr<size_t>(error, written);
            }

            written++;
        }
    }

    return ErrorOr<size_t>(written);
}

ErrorOr<size_t> format(Stream& stream, const char* string, FormatInfo& info);
ErrorOr<size_t> format(Stream& stream, void* value, FormatInfo& info);
ErrorOr<size_t> format(Stream& stream, unsigned int value, FormatInfo& info);
ErrorOr<size_t> format(Stream& stream, Formattable& value, FormatInfo& info);
ErrorOr<size_t> format(Stream& stream, Formattable* value, FormatInfo& info);

}