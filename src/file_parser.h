#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

struct graph *makegraph(const char *filename);
void delgraph(struct graph *g);

#endif
