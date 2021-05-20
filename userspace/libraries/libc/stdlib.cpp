#include <libc/stdlib.h>
#include <libc/math.h>
#include <libc/string.h>

#include <libsystem/Macros.h>
#include <libsystem/Assert.h>

unsigned long int strtoul(const char* str, char** endptr, const int base)
{
	// TODO: read standard for endptr behavior
	// TODO: parse more than base 10
	((void)endptr);
	((void)base);
	size_t lastDigI = 0;
	size_t len = strlen(str);
	for(size_t i = 0; i < len; i++) {
		char j = 0;
		for(; j <= base+1; j++) {
			if(str[i] == '0' + j) { lastDigI++; break; }
		}
		if(j >= 10) break;
	}

	unsigned long int value = 0;
	for(size_t i = lastDigI; i > 0; i--) {
		char digitVal = str[i-1] - '0';
		long long int mod = ullpow(base, lastDigI - i);
		value += mod * digitVal;
	}

	if(endptr != NULL)
		*endptr = (char*)str+lastDigI;

	return value;
}

extern "C" void _exit(int status) __noreturn;
void exit(int status)
{
	#ifndef __KERNEL__
	_exit(status);
	#else
	__unused(status);
	assert_not_reached();
	#endif
}
