#include "utility.h"

void *myalloc(size_t sz) {
    void *ptr;
    ptr = malloc(sz);
    if (!ptr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int is_number(char *argv)
{
    if (!argv) return 0;
    if (*argv == '\0') return 0;

    int i = 0;

    while (argv[i] != '\0')
        if (!isdigit(argv[i++])) return 0;

    return 1;
}

int contains(int needle, int *haystack, int sz) {
    while (sz--) {
        if (*(haystack + sz) == needle) return 1;
    }
    return 0;
}

