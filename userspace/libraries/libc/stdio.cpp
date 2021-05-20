#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <libsystem/__plugs__.h>

FILE* stdout = (FILE*)hegel::plugs::out_stream;
FILE* stdin = (FILE*)hegel::plugs::in_stream;
FILE* stderr = (FILE*)hegel::plugs::err_stream;

size_t fwrite(const void* buf, size_t size, size_t count, FILE* file)
{
    auto* stream = reinterpret_cast<hegel::Stream*>(file);
    return stream->write(buf, size * count).unwrap();
}

int putchar(int c)
{
    return putc(c, stdout);
}

/*int putc(int c, FILE* file)
{
    char b = c;
    fwrite(&b, 1, 1, file);
    return c;
}*/

int fputc(int c, FILE* file) 
{ 
    char b = c;
    fwrite(&b, 1, 1, file);
    return c;
}

int puts(const char* s)
{
    auto written = fwrite(s, strlen(s), 1, stdout);
    written += fwrite("\n", 1, 1, stdout);
    
    return written <= 0 ? -1 : written;
}

int printf(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = vfprintf(stdout, fmt, va);
    va_end(va);
    return result;
}

int fprintf(FILE* f, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = vfprintf(f, fmt, va);
    va_end(va);
    return result;
}

int sprintf(char* buf, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = vsnprintf(buf, -1, fmt, va);
    va_end(va);

    return result;
}

int snprintf(char* buf, size_t size, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = vsnprintf(buf, size, fmt, va);
    va_end(va);

    return result;
}

int vsprintf(char* buf, const char* fmt, va_list args)
{
    return vsnprintf(buf, -1, fmt, args);
}
