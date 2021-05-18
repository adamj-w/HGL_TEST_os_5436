#include <libc/stdio.h>

#include <libsystem/Stdio.h>

FILE* stdout = (FILE*)&hegel::stdout;
FILE* stdin = (FILE*)&hegel::stdin;
FILE* stderr = (FILE*)&hegel::stderr;

int putchar(int c)
{
    return hegel::stdout->write(&c, 1).unwrap();
}
