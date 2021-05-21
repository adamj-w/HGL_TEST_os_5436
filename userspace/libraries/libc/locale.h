#ifndef LOCALE_H
#define LOCALE_H 1

#include "bits/libc.h"

__BEGIN_HEADER

struct lconv {
	char* decimal_point;
	char* thousands_sep;
	char* grouping;
	char* int_curr_symbol;
	char* currency_symbol;
	char* mon_decimal_point;
	char* mon_thousands_sep;
	char* mon_grouping;
	char* positive_sign;
	char* negative_sign;
	char frac_digits;
	char p_cs_precedes;
	char n_cs_precedes;
	char p_sep_by_space;
	char n_sep_by_space;
	char p_sign_posn;
	char n_sign_posn;
	char int_frac_digits;
	char int_p_cs_precedes;
	char int_n_cs_precedes;
	char int_p_sep_by_space;
	char int_n_sep_by_space;
	char int_p_sign_posn;
	char int_n_sign_posn;
};

typedef struct lconv locale_t;

#define LC_ALL (0)
#define LC_COLLATE (1)
#define LC_CTYPE (2)
#define LC_MONETARY (3)
#define LC_NUMERIC (4)
#define LC_TIME (5)

locale_t newlocale(int category, const char* locale, locale_t base);
locale_t duplocale(locale_t t);
void freelocale(locale_t t);
locale_t uselocale(locale_t t);

char* setlocale(int caregory, const char* locale);
struct lconv* localeconv(void);

__END_HEADER

#endif