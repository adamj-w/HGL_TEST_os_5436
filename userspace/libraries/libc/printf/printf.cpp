#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <string.h>

#include <printf/printf.h>

void file_append(printf_info_t* info, char c)
{
    fputc(c, (FILE*)info->output);
}

int vfprintf(FILE* file, const char* fmt, va_list va)
{
    printf_info_t info = {};

    info.format = fmt;
    info.append = file_append;
    info.output = file;
    info.allocated = -1;

    return __printf(&info, va);
}

int vprintf(const char* fmt, va_list va)
{
    return vfprintf(stdout, fmt, va);
}

void buf_append(printf_info_t* info, char c)
{
    if(info->allocated == (size_t)-1) {
        strapd((char*)info->output, c);
    } else {
        strnapd((char*)info->output, c, info->allocated);
    }
}

int vsnprintf(char* s, size_t n, const char* fmt, va_list va)
{
    printf_info_t info = {};

    info.format = fmt;
    info.append = buf_append;
    info.allocated = n;
    info.output = s;

    return __printf(&info, va);
}
