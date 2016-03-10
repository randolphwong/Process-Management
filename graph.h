#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "utility.h"

#define MAX_LENGTH 80
#define MAX_CHILDREN 10

#define INELIGIBLE 1
#define READY 2
#define RUNNING 3
#define FINISHED 4

struct node {
    int id;
    char prog[MAX_LENGTH];
    char input[MAX_LENGTH];
    char output[MAX_LENGTH];
    int parent[MAX_CHILDREN];
    int parent_count;
    int children[MAX_CHILDREN];
    int child_count;
    int status;
    pid_t pid;
};

int is_singlerooted(struct node *graph, int sz);
int has_cycles(struct node *graph, struct node *n, int *visited, int visits);
int is_dag(struct node *graph, int sz);

#endif