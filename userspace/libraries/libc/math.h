#ifndef MATH_H
#define MATH_H 1

#ifdef __cplusplus
extern "C" {
#endif

long long int llpow(long long x, long long y);
unsigned long long int ullpow(unsigned long long x, long long y);

// Special instructions in some fpu s
#  define isgreater(x, y)	__builtin_isgreater(x, y)
#  define isgreaterequal(x, y)	__builtin_isgreaterequal(x, y)
#  define isless(x, y)		__builtin_isless(x, y)
#  define islessequal(x, y)	__builtin_islessequal(x, y)
#  define islessgreater(x, y)	__builtin_islessgreater(x, y)
#  define isunordered(x, y)	__builtin_isunordered(x, y)

#ifdef __cplusplus
}
#endif

#endif
