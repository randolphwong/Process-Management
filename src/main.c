/*
 * Programming Assignment 1
 * Author   : Chiew Jun Hao (1001046)
 * Author   : Wong Wai Kit Randolph (1001043)
 * Date     : 10/03/2016
 */

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "file_parser.h"
#include "proc_management.h"

int main(int argc, char **argv) {

    struct graph *proc_graph;
    char *filename;

    if (argc > 1) {
        filename = argv[1];
    } else {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    proc_graph = makegraph(filename);

    if (is_valid_graph(proc_graph)) {
        startproc(proc_graph);
    } else {
        fprintf(stderr, "invalid graph (no cycles or unrooted graph allowed).\n");
    }

    delgraph(proc_graph);

    return 0;
}

