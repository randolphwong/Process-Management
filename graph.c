#include "graph.h"

/**
 * is_singlerooted - determines whether a graph is singly rooted
 *
 * returns 1 if a graph is singly rooted, and 0 otherwise.
 */
int is_singlerooted(struct node *graph, int sz)
{
    int root_count = 0;

    while (sz--) {
        if ((graph+sz)->parent_count == 0) root_count++;
    }
    return root_count == 1;

}

/**
 * has_cycles - determines whether a graph contains cycles
 *
 * returns 1 if a graph contains cycle, and 0 otherwise.
 */
int has_cycles(struct node *graph, struct node *n, int *visited, int visits)
{
    if (contains(n->id, visited, visits)) return 1;
    visited[visits++] = n->id;

    int i;
    int ret = 0;

    for (i = 0; i < n->child_count; i++) {
        ret |= has_cycles(graph, &graph[n->children[i]], visited, visits);
    }

    return ret;
}

/**
 * is_dag - determines whether a graph is a directed acyclic graph
 *
 * returns 1 if a graph is a DAG, and 0 otherwise.
 */
int is_dag(struct node *graph, int sz)
{
    if (sz < 1) return 0;

    int i;
    int visited[sz];
    for (i = 0; i < sz; i++) {
        visited[i] = -1;
    }

    return is_singlerooted(graph, sz) &&
            !has_cycles(graph, &graph[0], visited, 0);
}

