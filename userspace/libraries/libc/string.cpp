#include <string.h>

#include <stdint.h>

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

int memcmp(const void* b1, const void* b2, size_t n)
{
	const uint8_t* s1 = (uint8_t*)b1;
	const uint8_t* s2 = (uint8_t*)b2;

	for(size_t i = 0; i < n; i++) {
		int cmp = s1[i] - s2[i];
		if(cmp != 0) return cmp;
	}

	return 0;
}

int strcmp(const char* s1, const char* s2)
{
	while((s1[0] && s2[0]) && s1[0] == s2[0]) {
		s1++, s2++;
	}

	return s1[0] - s2[0];
}

int strncmp(const char* s1, const char* s2, size_t n)
{
	size_t i = 0;

	while((s1[i] && s2[i]) && s1[i] == s2[i]) {
		i++;
		if(i >= n) break;
	}

	return s1[i] - s2[i];
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

void strnapd(char* str, char c, size_t n)
{
    for(size_t i = 0; i < (n - 1); i++) {
		if(str[i] == '\0') {
			str[i] = c;
			str[i + 1] = '\0';
			return;
		}
	}
}

char* strcat(char* s1, const char* s2)
{
	size_t dest_len = strlen(s1);
	size_t i;

	for (i = 0; s2[i] != '\0'; i++) {
		s1[dest_len + i] = s2[i];
	}

	s1[dest_len + i] = '\0';
	return s1;
}

char* strncat(char* s1, const char* s2, size_t n)
{
	size_t dest_len = strlen(s1);
	size_t i;

	for (i = 0 ; i < n && s2[i] != '\0' ; i++) {
		s1[dest_len + i] = s2[i];
	}

	s1[dest_len + i] = '\0';
	return s1;
}

void strrvs(char* str)
{
    for(size_t i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        char tmp = str[j];
        str[j] = str[i];
        str[i] = tmp;
    }
}

char* strncpy(char* dst, const char* src, size_t n)
{
	size_t i = 0;
	while(src[i] && i <= n) {
		dst[i] = src[i];
		i++;
	}
	return dst;
}
