#ifndef UTILITY_H
#define UTILITY_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error_en(en, msg) \
       do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
       do { perror(msg); exit(EXIT_FAILURE); } while (0)


void *myalloc(size_t sz);
int is_number(char *argv);
int contains(int needle, int *haystack, int sz);

#endif
