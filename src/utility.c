#include "utility.h"

/**
 * is_number - checks whether the argument string is a number
 *
 * returns 1 if it is a number, and 0 otherwise.
 */
int is_number(const char *s)
{
    if (!s) return 0;
    if (*s == '\0') return 0;

    int i = 0;

    while (s[i] != '\0')
        if (!isdigit(s[i++])) return 0;

    return 1;
}

/**
 * contains - find a needle in the haystack
 *
 * returns 1 if the needle is found, and 0 otherwise.
 */
int contains(int needle, int *haystack, int sz)
{
    while (sz--) {
        if (*(haystack + sz) == needle) return 1;
    }
    return 0;
}

