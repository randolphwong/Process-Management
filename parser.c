#include "parser.h"

char *nextarg(struct Command *cmd)
{
    char *argv = NULL;
    if (cmd->args) {
        argv = cmd->args->argv;
        cmd->args = cmd->args->next;
    }
    return argv;
}

struct Command *makecommand(const char *s)
{
    struct Command *cmd = myalloc(sizeof(*cmd));
    struct Arg *prev_arg;
    struct Arg *next_arg;
    char *tmp;
    char *argv;
    char *token;

    tmp = strdup(s);

    token = strtok(tmp, " \n");
    if (token) {
        prev_arg = myalloc(sizeof(*prev_arg));
        argv = strdup(token);

        prev_arg->argv = argv;
        prev_arg->next = NULL;
        cmd->args = prev_arg;
        cmd->head = prev_arg;
    } else {
        free(cmd);
        free(tmp);
        return NULL;
    }

    while ((token = strtok(NULL, " \n"))) {
        next_arg = myalloc(sizeof(*next_arg));
        argv = strdup(token);

        next_arg->argv = argv;
        next_arg->next = NULL;
        prev_arg->next = next_arg;
        prev_arg = next_arg;
    }

    free(tmp);

    return cmd;
}

void delcommand(struct Command *cmd)
{
    struct Arg *curr;

    while ((curr = cmd->head)) {
        cmd->head = cmd->head->next;
        free(curr->argv);
        free(curr);
    }
    free(cmd);
}

