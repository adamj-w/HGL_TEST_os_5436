#include <libc/string.h>

void* memcpy(void* dst, const void* src, size_t n)
{
	unsigned int* dstp = (unsigned int*)dst;
	unsigned int* srcp = (unsigned int*)src;

	while(n >= sizeof(unsigned int))
	{
		*dstp++ = *srcp++;
		n -= sizeof(unsigned int);
	}

	char* dstc = (char*)dstp;
	char* srcc = (char*)srcp;

	while(n >= sizeof(char))
	{
		*dstc++ = *srcc++;
		n -= sizeof(char);
	}

	return dst;
}

void* memset(void* s, int c, size_t n) 
{
	for(size_t i = 0; i < n; ++i) {
		((char*)s)[i] = c;
	}

	return s;
}

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