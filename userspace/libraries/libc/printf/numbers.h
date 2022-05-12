#pragma once

#include <stddef.h>

struct NumberFormatter
{
    int base;
    int after_point;
    bool capitalized;
};

#define FORMAT_BINARY ((NumberFormatter{2, 4, false}))
#define FORMAT_OCTAL ((NumberFormatter{8, 4, false}))
#define FORMAT_DECIMAL ((NumberFormatter{10, 4, false}))
#define FORMAT_HEXADECIMAL ((NumberFormatter{16, 4, false}))

size_t format_uint(NumberFormatter format, unsigned long value, char* str, size_t size);
size_t format_int(NumberFormatter format, long value, char* str, size_t size);
size_t format_float(NumberFormatter format, double value, char* str, size_t size);
