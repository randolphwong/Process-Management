#include "file_parser.h"

#define NODE_LIMIT 2

/**
 * set_node_prog - set the node's corresponding program and arguments
 */
static void set_node_prog(struct node *n, char *prog_tok)
{
    strcpy(n->prog, prog_tok);
}

/**
 * set_node_input - set the node's corresponding input
 *
 * error:
 * 1. no input in the input token
 */
static void set_node_input(struct node *n, char *input_tok)
{
    input_tok = strtok(input_tok, " ");
    if (!input_tok) {
        fprintf(stderr, "no input found in process %d.\n", n->id);
        exit(EXIT_FAILURE);
    }

    strcpy(n->input, input_tok);
}

/**
 * set_node_output - set the node's corresponding output
 *
 * error:
 * 1. no output in the output token
 */
static void set_node_output(struct node *n, char *output_tok)
{
    output_tok = strtok(output_tok, " ");
    if (!output_tok) {
        fprintf(stderr, "no output found in process %d.\n", n->id);
        exit(EXIT_FAILURE);
    }

    strcpy(n->output, output_tok);
}

/**
 * set_node_children - set the node's corresponding children and child_count
 *
 * error:
 * 1. no children in the children token
 * 2. invalid children (non number or negative numbers)
 */
static void set_node_children(struct node *n, char *child_tok)
{
    int child_id;

    child_tok = strtok(child_tok, " ");
    if (!child_tok) {
        fprintf(stderr, "no children found in process %d.\n", n->id);
        exit(EXIT_FAILURE);
    }

    while (child_tok) {
        if (strcmp(child_tok, "none") == 0) {
            break;
        } else if (is_number(child_tok)) {
            child_id = atoi(child_tok);
            n->children[n->child_count++] = child_id;
        } else {
            fprintf(stderr, "invalid children for process %d.\n", n->id);
            exit(EXIT_FAILURE);
        }
        child_tok = strtok(NULL, " ");
    }
}

/**
 * set_node_parent - set the node's corresponding parent and parent_count
 *
 * error:
 * 1. one of the node has a children whose id cannot be found
 */
static void set_node_parent(struct graph *g)
{
    int i, j;
    struct node *parent;
    struct node *child;

    for (i = 0; i < g->sz; i++) {
        parent = g->procs[i];
        if (parent->child_count > 0) {
            for (j = 0; j < parent->child_count; j++) {
                if (parent->children[j] < g->sz) {
                    child = g->procs[parent->children[j]];
                    child->parent[child->parent_count++] = parent->id;
                } else {
                    fprintf(stderr, "child id %d not found.\n", parent->children[j]);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

/**
 * set_graph_roots - set the graph roots and root_count
 */
static void set_graph_roots(struct graph *g)
{
    int i;
    struct node *n;

    g->roots = malloc(sizeof(*g->roots) * g->sz);
    if (!g->roots)
        handle_error("malloc");

    for (i = 0; i < g->sz; i++) {
        n = g->procs[i];
        if (n->parent_count == 0) {
            g->roots[g->root_count++] = n;
            n->status = READY;
        }
    }
}

static void expand_graph(struct node ***procs, int new_sz)
{
    *procs = realloc(*procs, sizeof(**procs) * new_sz);
    if (!*procs)
        handle_error("realloc");
}

/**
 * makegraph - read the specified file in the argument and creates a graph out
 * of it
 * 
 * Returns a graph on success, and otherwise it will terminate the program.
 *
 * error:
 * 1. missing token (total count of colon delimiter is less than 3)
 * 2. empty file
 */
struct graph *makegraph(const char *filename)
{
    struct graph *proc_graph = malloc(sizeof(*proc_graph));
    struct node *current_node;
    int max_graph_sz;
    int nodeid;

    char *prog_token;
    char *child_token;
    char *input_token;
    char *output_token;
    char nextline[100];

    FILE *fptr;

    if (!proc_graph)
        handle_error("malloc");

    if(!(fptr = fopen(filename, "r")))
        handle_error("fopen");

    proc_graph->sz = 0;
    max_graph_sz = NODE_LIMIT;
    proc_graph->procs = malloc(sizeof(*proc_graph->procs) * NODE_LIMIT);
    if (!proc_graph->procs)
        handle_error("malloc");

    for (nodeid = 0; fgets(nextline, sizeof(nextline), fptr); nodeid++) {

        if (is_empty_line(nextline)) {
            nodeid--;
            continue;
        }

        proc_graph->procs[nodeid] = calloc(1, sizeof(**proc_graph->procs));
        current_node = proc_graph->procs[nodeid];
        if (!current_node)
            handle_error("calloc");

        prog_token = strtok(nextline, ":");
        child_token = strtok(NULL, ":");
        input_token = strtok(NULL, ":");
        output_token = strtok(NULL, ":\n");

        if (prog_token == NULL || child_token == NULL ||
            input_token == NULL || output_token == NULL) {
            fprintf(stderr, "missing token on line %d.\n", nodeid + 1);
            exit(EXIT_FAILURE);
        }

        current_node->id = nodeid;
        current_node->status = INELIGIBLE;

        set_node_prog(current_node, prog_token);
        set_node_input(current_node, input_token);
        set_node_output(current_node, output_token);
        set_node_children(current_node, child_token);

        if (nodeid >= max_graph_sz) {
            max_graph_sz <<= 1;
            expand_graph(&proc_graph->procs, max_graph_sz);
        }
    }

    if (nodeid < 1) {
        fprintf(stderr, "empty file.\n");
        exit(EXIT_FAILURE);
    } else {
        proc_graph->sz = nodeid;
    }

    set_node_parent(proc_graph);
    set_graph_roots(proc_graph);

    if ((fclose(fptr)) != 0)
        handle_error("fclose");

    return proc_graph;
}

/**
 * delgraph - free all memory allocation made by makegraph
 */
void delgraph(struct graph *g)
{
    if(g) {
        free(g->roots);
        free(g->procs);
    }
    free(g);
}

