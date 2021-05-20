#include "numbers.h"

#include <string.h>

static const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char* capital_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

size_t format_uint(NumberFormatter format, unsigned long value, char* str, size_t size)
{
    if(size == 0) {
        return 0;
    }

    size_t written = 0;

    str[0] = '\0';

    do {
        if(format.capitalized) {
            strnapd(str, capital_digits[value % format.base], size);
        } else {
            strnapd(str, digits[value % format.base], size);
        }

        value /= format.base;
        written++;
    } while(value != 0);

    strrvs(str);

    return written;
}

size_t format_int(NumberFormatter format, long value, char* str, size_t size)
{
    if(size == 0) {
        return 0;
    }

    size_t written = 0;

    str[0] = '\0';

    if(value < 0) {
        strnapd(str, '-', size);
        str++;
        size--;
        value = -value;
        written++;
    }

    return written + format_uint(format, value, str, size);
}
