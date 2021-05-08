#include <libc/string.h>

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len]) len++;
    return len;
}

void strapd(char* str, char c)
{
    size_t len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

void strrvs(char* str)
{
    for(size_t i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        char tmp = str[j];
        str[j] = str[i];
        str[i] = tmp;
    }
}