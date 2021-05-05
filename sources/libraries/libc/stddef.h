#ifndef STDDEF_H
#define STDDEF_H 1

#include "bits/NULL.h"

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

#define offsetof(type, member) ((size_t)&(((type*)0)->member))

#endif