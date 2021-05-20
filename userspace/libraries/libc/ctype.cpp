#include "ctype.h"

int isdigit(int c) {
    return '0' <= c && c <= '9' ? 1 : 0;
}

int islower(int c) {
    return 'a' <= c && c <= 'z' ? 1 : 0;
}

int isupper(int c) {
    return 'A' <= c && c <= 'Z' ? 1 : 0;
}

int isalpha(int c) {
    return islower(c) || isupper(c) ? 1 : 0;
}
