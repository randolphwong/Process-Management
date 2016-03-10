#ifndef PARSER_H
#define PARSER_H

#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

struct Arg {
    char *argv;
    struct Arg *next;
};

struct Command {
    struct Arg *args;
    struct Arg *head;
};

char *nextarg(struct Command *);
struct Command *makecommand(const char *);
void delcommand(struct Command *);

#endif
