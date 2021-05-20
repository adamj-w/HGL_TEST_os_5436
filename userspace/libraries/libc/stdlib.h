#ifndef STDLIB_H
#define STDLIB_H 1

#include <stddef.h>
#include "bits/libc.h"

__BEGIN_HEADER

// Exit codes
#define EXIT_FAILURE (-1)
#define EXIT_SUCCESS (0)

// Numeric conversion functions
int atoi(const char* s);
long int atol(const char* s);
long long int atoll(const char* s);
float atof(const char* s);

long int strtol(const char* str, char** endptr, int base);
long long int strtoll(const char* str, char** endptr, int base);
unsigned long int strtoul(const char* str, char** endptr, int base);
unsigned long long int strtoull(const char* str, char** endptr, int base);

float strtof(const char* str, char** endptr);
double strtod(const char* str, char** endptr);
long double strtold(const char* str, char** endptr);

// Random number generation
#define RAND_MAX (__INT_MAX__)
int rand(void);
void srand(unsigned int seed);

// Memory management functions
extern void* aligned_alloc(size_t alignment, size_t size) __attribute__((alloc_size(2)));
extern void* calloc(size_t nmemb, size_t size) __attribute__((alloc_size(1,2)));
extern void free(void* ptr);
extern void* malloc(size_t size) __attribute__((alloc_size(1)));
extern void* realloc(void* ptr, size_t size) __attribute__((alloc_size(2)));

// Environment
void abort() __attribute__((__noreturn__));
int atexit(void (*func)(void));
void exit(int status) __attribute__((__noreturn__));
char* getenv(const char* name);
int system(const char* command);

// Array algorithms
void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*, const void*));
void qsort(void* base, size_t num, size_t size, int (*compar)(const void*, const void*));

// Integer math
typedef struct {
	int quot;
	int rem;
} div_t;
typedef struct {
	long quot;
	long rem;
} ldiv_t;
typedef struct {
	long long quot;
	long long rem;
} lldiv_t;

int abs(int n);
long labs(long n);
long long llabs(long long n);
div_t div(int number, int denom);
ldiv_t ldiv(long number, long denom);
lldiv_t lldiv(long long number, long long denom);

__END_HEADER

#endif