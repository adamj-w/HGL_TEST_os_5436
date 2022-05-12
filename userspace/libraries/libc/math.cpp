#include <libc/math.h>

#include <stdint.h>
#include <limits.h>

#define FM_DOUBLE_LOG2OFE  1.4426950408889634074
#define FM_DOUBLE_LOGEOF2  6.9314718055994530942e-1
#define FM_DOUBLE_LOG2OF10 3.32192809488736234789
#define FM_DOUBLE_LOG10OF2 3.0102999566398119521e-1
#define FM_DOUBLE_LOG10OFE 4.3429448190325182765e-1
#define FM_DOUBLE_SQRT2    1.41421356237309504880
#define FM_DOUBLE_SQRTH    0.70710678118654752440

static const double fm_exp2_q[] __attribute__ ((aligned(16))) = {
/*  1.00000000000000000000e0, */
    2.33184211722314911771e2,
    4.36821166879210612817e3
};
static const double fm_exp2_p[] __attribute__ ((aligned(16))) = {
    2.30933477057345225087e-2,
    2.02020656693165307700e1,
    1.51390680115615096133e3
};

typedef union {
	double f;
	uint64_t u;
	struct {int32_t i0,i1;} s;
} udi_t;
#define FM_DOUBLE_BIAS 1023
#define FM_DOUBLE_EMASK 2146435072
#define FM_DOUBLE_MBITS 20
#define FM_DOUBLE_MMASK 1048575
#define FM_DOUBLE_EZERO 1072693248

extern "C" double floor(double num) {
    if (num >= (double)LLONG_MAX || num <= (double)LLONG_MIN || num != num) {
        /* handle large values, infinities and nan */
        return num;
    }
    long long n = (long long)num;
    double d = (double)n;
    if (d == num || num >= 0)
        return d;
    else
        return d - 1;
}

extern "C" double exp2(double x)
{
    double   ipart, fpart, px, qx;
    udi_t    epart;

    ipart = __builtin_floor(x+0.5);
    fpart = x - ipart;
	epart.s.i0 = 0;
	epart.s.i1 = (((int) ipart) + 1023) << 20;

    x = fpart*fpart;

    px =        fm_exp2_p[0];
    px = px*x + fm_exp2_p[1];
    qx =    x + fm_exp2_q[0];
    px = px*x + fm_exp2_p[2];
    qx = qx*x + fm_exp2_q[1];

    px = px * fpart;

    x = 1.0 + 2.0*(px/(qx-px));
    return epart.f*x;
}

extern "C" double exp(double x)
{
    return exp2(FM_DOUBLE_LOG2OFE*x);
}

extern "C" double exp10(double x)
{
    return exp2(FM_DOUBLE_LOG2OF10*x);
}

#include "bits/logspline_tbl.inl"
extern "C" double log(double x) 
{
    udi_t val;
    double a,b,y;
    int32_t hx, ipart;

    val.f = x;
    hx = val.s.i1;
    
    /* extract exponent and subtract bias */
    ipart = (((hx & FM_DOUBLE_EMASK) >> FM_DOUBLE_MBITS) - FM_DOUBLE_BIAS);

    /* mask out exponent to get the prefactor to 2**ipart */
    hx &= FM_DOUBLE_MMASK;
    val.s.i1 = hx | FM_DOUBLE_EZERO;
    x = val.f;

    /* table index */
    hx >>= FM_SPLINE_SHIFT;

    /* compute x value matching table index */
    val.s.i0 = 0;
    val.s.i1 = FM_DOUBLE_EZERO | (hx << FM_SPLINE_SHIFT);
    b = (x - val.f) * fm_log_dinv;
    a = 1.0 - b;

    /* evaluate spline */
    y = a * fm_log_q1[hx] + b * fm_log_q1[hx+1];
    a = (a*a*a-a) * fm_log_q2[hx];
    b = (b*b*b-b) * fm_log_q2[hx+1];
    y += (a + b) * fm_log_dsq6;

    return ((double)ipart)*FM_DOUBLE_LOGEOF2 + y;
}

extern "C" double pow(double x, double y) {
	// TODO: bad
	return exp(y * log(x));
}

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
