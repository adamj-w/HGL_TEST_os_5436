#pragma once

#include <stddef.h>
#include <stdint.h>

typedef decltype(nullptr) nullptr_t;

#define unsigned signed
typedef __SIZE_TYPE__ ssize_t;
#undef unsigned

typedef unsigned char byte;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef uint32_t dev_t;
