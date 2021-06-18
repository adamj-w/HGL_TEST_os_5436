#ifndef STRING_H
#define STRING_H 1

#include <stddef.h>

#include "bits/libc.h"

__BEGIN_HEADER

// Copying functions
void* memcpy(void* dst, const void* src, size_t n);
void* memmove(void* dst, const void* src, size_t n);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);

// Concatenation functions
char* strcat(char* s1, const char* s2);
char* strncat(char* s1, const char* s2, size_t n);
void strapd(char* s, char c);
void strnapd(char* s, char c, size_t n);

// Comparison functions
int memcmp(const void* s1, const void* s2, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
int strcoll(const char* s1, const char* s2);
size_t strxfrm(char* s1, const char* s2, size_t n);

// Search functions
void* memchr(const void* s, int c, size_t n);
char* strchr(const char* s, int c);
char* strrchr(const char* s, int c);
size_t strspn(const char* s1, const char* s2);
size_t strcspn(const char* s1, const char* s2);
char* strpbrk(const char* s1, const char* s2);
char* strstr(const char* s1, const char* s2);
char* strtok(char* s1, const char* s2);

// Miscellaneous functions
void* memset(void* s, int c, size_t n);
char* strerror(int errnum);
size_t strlen(const char* s);
void strrvs(char* s);

__END_HEADER

#endif