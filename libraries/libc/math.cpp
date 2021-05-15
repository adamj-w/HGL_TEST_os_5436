#include <libc/math.h>

extern "C" unsigned long long int ullpow(const unsigned long long int x, const long long int y) {
	long long z = 1;
	if(y > 0) {
		for(long long i = 0; i < y; i++) {
			z *= x;
		}
	}
	if(y < 0 && x != 0) {
		return 0;
	}

	return z;
}

extern "C" long long int llpow(const long long int x, const long long int y) {
	long long z = 1;
	if(y > 0) {
		for(long long i = 0; i < y; i++) {
			z *= x;
		}
	}
	if(y < 0 && x != 0) {
		return 0;
	}

	return z;
}
