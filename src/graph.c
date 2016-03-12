#include "graph.h"

/**
 * print_graph - prints the graph information
 */
void print_graph(struct graph *g)
{
    int i, j;
    struct node *n;

    printf("graph size: %d\n", g->sz);
    printf("graph roots:");
    for (i = 0; i < g->root_count; i++)
        printf("%d ", g->roots[i]->id);
    printf("\ngraph root_count: %d\n\n", g->root_count);

    for (i = 0; i < g->sz; i++) {
        n = g->procs[i];
        printf("[node %d] prog: %s\n", n->id, n->prog);
        printf("[node %d] children: ", n->id);
        for (j = 0; j < n->child_count; j++)
            printf("%d ", n->children[j]);
        printf("\n[node %d] child_count: %d\n", n->id, n->child_count);
        printf("[node %d] input: %s\n", n->id, n->input);
        printf("[node %d] output: %s\n", n->id, n->output);
        printf("[node %d] status: %d\n\n", n->id, n->status);
    }
}

/**
 * graph_has_cycles - checks for cycle starting from node n
 *
 * Returns 1 if a cycle exists, and 0 otherwise.
 */
static int 
graph_has_cycles(struct graph *g, struct node *n, int *visited, int visits)
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

/**
 * has_cycles - checks for cycle starting from all roots
 *
 * returns 1 if a graph contains cycle, and 0 otherwise.
 */
int has_cycles(struct graph *g)
{
    int i;
    int visited[g->sz];

    for (i = 0; i < g->root_count; i++) {
        if (graph_has_cycles(g, g->roots[i], visited, 0)) return 1;
    }

    return 0;
}

/**
 * has_roots - checks whether the graph(s) is rooted or not
 *
 * returns 1 if the graph has at least one root, and 0 otherwise.
 */
int has_roots(struct graph *g)
{
    return g->root_count > 0;
}

/**
 * is_valid_graph - checks the validity of the graph (no cycles)
 *
 * returns 1 if the graph is valid, and 0 otherwise.
 */
int is_valid_graph(struct graph *g)
{
    return has_roots(g) && !has_cycles(g);
}
