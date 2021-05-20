#ifndef CTYPE_H
#define CTYPE_H 1

#include "bits/libc.h"

#include <locale.h>

__BEGIN_HEADER

int isblank(int c);
int isblank_l(int c, locale_t l);

int iscntrl(int c);
int iscntrl_l(int c, locale_t l);

int isupper(int c);
int isupper_l(int c, locale_t l);
int islower(int c);
int islower_l(int c, locale_t l);
int isalpha(int c);
int isalpha_l(int c, locale_t l);
int isdigit(int c);
int isdigit_l(int c, locale_t l);
int isxdigit(int c);
int isxdigit_l(int c, locale_t l);
int isalnum(int c);
int isalnum_l(int c, locale_t l);
int isgraph(int c);
int isgraph_l(int c, locale_t l);
int isprint(int c);
int isprint_l(int c, locale_t l);
int isspace(int c);
int isspace_l(int c, locale_t l);

int isascii(int c);
int toascii(int c);

int ispunct(int c);
int ispunct_l(int c, locale_t l);

int tolower(int c);
int tolower_l(int c, locale_t l);
int toupper(int c);
int toupper_l(int c, locale_t l);

#define _toupper(x) (toupper(x))
#define _tolower(x) (tolower(x))

__END_HEADER

#endif