/*
 * Command is a linked list constructed from a given string. Each element
 * corresponds to the tokens (arguments) in the string.
 */

#include "parser.h"

/**
 * nextarg - get the next argument from Command
 */
char *nextarg(struct Command *cmd)
{
    char *argv = NULL;
    if (cmd->args) {
        argv = cmd->args->argv;
        cmd->args = cmd->args->next;
    }
    return argv;
}

/**
 * makecommand - create a Command from a string
 */
struct Command *makecommand(const char *s)
{
    struct Command *cmd = malloc(sizeof(*cmd));
    struct Arg *prev_arg;
    struct Arg *next_arg;
    char *tmp;
    char *argv;
    char *token;

    if (!cmd)
        handle_error("malloc");

    tmp = strdup(s);

    token = strtok(tmp, " \n");
    if (token) {
        prev_arg = malloc(sizeof(*prev_arg));
        if (!prev_arg)
            handle_error("malloc");

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
        next_arg = malloc(sizeof(*next_arg));
        if (!next_arg)
            handle_error("malloc");

        argv = strdup(token);

        next_arg->argv = argv;
        next_arg->next = NULL;
        prev_arg->next = next_arg;
        prev_arg = next_arg;
    }

    free(tmp);

    return cmd;
}

/**
 * delcommand - free all resources allocated by makecommand
 */
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

