#include "graph.h"

/**
 * has_cycles - determines whether a graph contains cycles
 *
 * returns 1 if a graph contains cycle, and 0 otherwise.
 */

int graph_has_cycles(struct graph *g, struct node *n, int *visited, int visits)
{
    if (contains(n->id, visited, visits)) return 1;
    visited[visits++] = n->id;

    int i;
    int ret = 0;

    for (i = 0; i < n->child_count; i++) {
        ret |= graph_has_cycles(g, g->procs[n->children[i]], visited, visits);
    }

    return ret;
}

int has_cycles(struct graph *g)
{
    int i;
    int visited[g->sz];

    for (i = 0; i < g->root_count; i++) {
        if (graph_has_cycles(g, g->roots[i], visited, 0)) return 1;
    }

    return 0;
}

int has_roots(struct graph *g)
{
    return g->root_count > 0;
}

int is_valid_graph(struct graph *g)
{
    return !has_cycles(g) && has_roots(g);
}
