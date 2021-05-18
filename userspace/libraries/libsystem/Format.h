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

ErrorOrSizeT format(Stream& stream, const char* fmt);

template<typename First, typename... Args>
ErrorOrSizeT format(Stream& stream, const char* fmt, First first, Args... args)
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

            ErrorOrSizeT res_format = format(stream, first, info);

            written += res_format.value();

            if(res_format != Error::SUCCEED) {
                return ErrorOrSizeT(res_format.error(), written);
            }

            ErrorOrSizeT res_print = format(stream, &fmt[i+1], args...);

            written += res_print.value();

            if(res_print != Error::SUCCEED) {
                return ErrorOrSizeT(res_print.error(), written);
            } else {
                return ErrorOrSizeT(written);
            }
        } else {
            auto error = stream.write_byte(fmt[i]);

            if(error != Error::SUCCEED) {
                return ErrorOrSizeT(error, written);
            }

            written++;
        }
    }

    return ErrorOrSizeT(written);
}

ErrorOrSizeT format(Stream& stream, const char* string, FormatInfo& info);
ErrorOrSizeT format(Stream& stream, const void* value, FormatInfo& info);
ErrorOrSizeT format(Stream& stream, unsigned int value, FormatInfo& info);
ErrorOrSizeT format(Stream& stream, Formattable& value, FormatInfo& info);
ErrorOrSizeT format(Stream& stream, Formattable* value, FormatInfo& info);

}