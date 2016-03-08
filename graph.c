#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 80
#define MAX_CHILDREN 10

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

int contains(int needle, int *haystack, int sz) {
    while (sz--) {
        if (*(haystack + sz) == needle) return 1;
    }
    return 0;
}

int is_singlerooted(struct node *graph, int sz) {
    int root_count = 0;

    while (sz--) {
        if ((graph+sz)->parent_count == 0) root_count++;
    }
    return root_count == 1;

}

int has_cycles(struct node *graph, struct node *n, int *visited, int visits) {
    if (contains(n->id, visited, visits)) return 1;
    visited[visits++] = n->id;

    int i;
    int ret = 0;

    for (i = 0; i < n->child_count; i++) {
        ret |= has_cycles(graph, &graph[n->children[i]], visited, visits);
    }

    return ret;
}

int is_dag(struct node *graph, int sz) {
    if (sz < 1) return 0;

    int i;
    int visited[sz];
    for (i = 0; i < sz; i++) {
        visited[i] = -1;
    }

    return is_singlerooted(graph, sz) &&
            !has_cycles(graph, &graph[0], visited, 0);
}

int main(void) {

    struct node graph[4];
    graph[0].id = 0;
    graph[0].parent_count = 0;
    graph[0].children[0] = 1;
    graph[0].children[1] = 2;
    graph[0].child_count = 2;

    graph[1].id = 1;
    graph[1].parent[0] = 0;
    graph[1].parent_count = 1;
    graph[1].children[0] = 3;
    graph[1].child_count = 1;

    graph[2].id = 2;
    graph[2].parent[0] = 0;
    graph[2].parent_count = 1;
    graph[2].children[0] = 3;
    graph[2].child_count = 1;

    graph[3].id = 3;
    graph[3].parent[0] = 1;
    graph[3].parent[1] = 2;
    graph[3].parent_count = 2;
    graph[3].child_count = 0;


    printf("is_dag: %d\n", is_dag(graph, 4));

    return 0;
}
