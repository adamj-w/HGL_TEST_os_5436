#ifndef STDIO_H
#define STDIO_H 1

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "bits/libc.h"

__BEGIN_HEADER

#define BUFSIZ 1024

typedef void* FILE;
typedef size_t fpos_t;

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define EOF (-1)

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

// File Control
FILE* fopen(const char* filename, const char* mode);
FILE* freopen(const char* filename, const char* mode, FILE* stream);
FILE* tmpfile();
char* tmpnam(char* str);
int fclose(FILE* stream);
int fflush(FILE* stream);

void setbuf(FILE* stream, char* buf);
int setvbuf(FILE* stream, char* buf, int mode, size_t size);
int rename(const char* oldstr, const char* newstr);
int remove(const char* file);

int fgetpos(FILE* stream, fpos_t* pos);
int fsetpos(FILE* stream, const fpos_t* pos);
int fseek(FILE* stream, long offset, int seek_mode);
long ftell(FILE* stream);
void rewind(FILE* stream);

// All Output functions
int putc(int c, FILE* stream);
int fputc(int c, FILE* stream);
int fputs(const char* s, FILE* stream);
size_t fwrite(const void* buf, size_t size, size_t nmemb, FILE* stream);

#define putc(c,s) fputc(c,s);
int putchar(int c);
int puts(const char* s);

int fprintf(FILE* stream, const char* format, ...);
int fnprintf(FILE* stream, size_t size, const char* fmt, ...);
int vfprintf(FILE* stream, const char* format, va_list arg);
int vfnprintf(FILE* stream, size_t size, const char* fmt, va_list args);

int printf(const char* format, ...);
int nprintf(size_t n, const char* format, ...);
int vprintf(const char* format, va_list arg);
int vnprintf(size_t n, const char* format, va_list arg);

int sprintf(char* buf, const char* fmt, ...);
int snprintf(char* buf, size_t size, const char* fmt, ...);
int vsprintf(char* buf, const char* fmt, va_list args);
int vsnprintf(char* buf, size_t size, const char* fmt, va_list args);

// All Input functions
int fgetc(FILE* stream);
int fgets(FILE* stream);
size_t fread(void* dst, size_t size, size_t count, FILE* stream);

int getchar();
int getc(FILE* stream);
char* gets(char* str);
int unget(int c, FILE* stream);

// Error handling
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* str);

__END_HEADER

#endif