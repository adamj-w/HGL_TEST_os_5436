#include <stdio.h>
#include <string.h>

#include <libsystem/__plugs__.h>

FILE* stdout = (FILE*)hegel::plugs::out_stream;
FILE* stdin = (FILE*)hegel::plugs::in_stream;
FILE* stderr = (FILE*)hegel::plugs::err_stream;

int putchar(int c)
{
    return hegel::plugs::out_stream->write(&c, 1).unwrap();
}

int puts(const char* s)
{
    auto error = hegel::plugs::out_stream->write(s, strlen(s));

    return !error.succeed() ? error.unwrap() : (error.value() + putchar('\n'));
}
