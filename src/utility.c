#include "utility.h"

/**
 * is_number - checks whether the argument string is a number
 *
 * Returns 1 if it is a number, and 0 otherwise.
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
 * Returns 1 if the needle is found, and 0 otherwise.
 */
int contains(int needle, int *haystack, int sz)
{
    while (sz--) {
        if (*(haystack + sz) == needle) return 1;
    }
    return 0;
}

/**
 * is_empty_line - checks whether the argument string consists of only
 * whitespace
 *
 * Returns 1 if string contains only whitespace, and 0 otherwise.
 */
int is_empty_line(const char *s)
{
    if (s == '\0') return 1;

    while (*s != '\0') {
        if (!isspace(*s++)) return 0;
    }

    return 1;
}

