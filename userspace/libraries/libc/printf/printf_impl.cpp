#include "printf.h"

#include <ctype.h>
#include <string.h>

#include <libsystem/Macros.h>

int __printf_format_string(printf_info_t* info, va_list* va)
{
    const char* v = va_arg(*va, char*);

    if(v == nullptr) {
        v = "(null)";
    }

    PRINTF_PADDING(v, PFALIGN_RIGHT);

    for(int i = 0; v[i]; i++) {
        PRINTF_APPEND(v[i]);
    }

    PRINTF_PADDING(v, PFALIGN_LEFT);

    return info->written;
}

static printf_formatter_t formatters[] = {
    { 's', __printf_format_string },
    { '\0', nullptr }
};

void __printf_format(printf_info_t* info, char c, va_list* va)
{
    for(int i = 0; formatters[i].c; i++) {
        if(formatters[i].c == c && formatters[i].impl != nullptr) {
            formatters[i].impl(info, va);
            return;
        }
    }

    const int trash = va_arg(*va, int);
    __unused(trash);
    info->append(info, '%');
    info->append(info, c);
}

int __printf(printf_info_t* info, va_list va)
{
    va_list args_copy;
    va_copy(args_copy, va);

    info->written = 0;
    info->format_offset = 0;
    info->state = PFSTATE_ESC;
    info->align = PFALIGN_RIGHT;
    info->padding = ' ';
    info->length = 0;

    if(info->format == nullptr) {
        for(int i = 0; "(null)"[i]; i++) {
            PRINTF_APPEND("(null)"[i]);
        }
        return 0;
    }

    PRINTF_PEEK();

    while(1) {
        switch(info->state) {
        case PFSTATE_ESC:
            if(info->c == '%') {
                info->state = PFSTATE_PARSE;
            } else {
                PRINTF_APPEND(info->c);
            }

            PRINTF_PEEK();
            break;
        case PFSTATE_PARSE: 
            if(info->c == '%') {
                info->state = PFSTATE_ESC;
                PRINTF_APPEND(info->c);
                PRINTF_PEEK();
            } else if(info->c == '0') {
                info->padding = '0';
                PRINTF_PEEK();
            } else if(info->c == '-') {
                info->align = PFALIGN_LEFT;
                PRINTF_PEEK();
            } else if(isdigit(info->c)) {
                info->state = PFSTATE_FORMAT_LENGTH;
            } else if(isalpha(info->c)) {
                info->state = PFSTATE_FINALIZE;
            } else {
                PRINTF_PEEK();
            }
            break;
        case PFSTATE_FORMAT_LENGTH:
            if(isdigit(info->c)) {
                info->length *= 10;
                info->length += info->c - '0';

                PRINTF_PEEK();
            } else if(isalpha(info->c)) {
                info->state = PFSTATE_FINALIZE;
            } else {
                info->state = PFSTATE_ESC;
            }
            break;
        case PFSTATE_FINALIZE:
            __printf_format(info, info->c, &args_copy);

            if(info->written == info->allocated) {
                return info->written;
            } else {
                info->length = 0;
                info->state = PFSTATE_ESC;
                info->padding = ' ';
                info->align = PFALIGN_RIGHT;

                PRINTF_PEEK();
            }
            break;

        default:
            break;
        }
    }

    va_end(args_copy);
}
