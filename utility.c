#include "utility.h"

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

