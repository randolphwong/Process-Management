#ifndef PROC_MANAGEMENT_H
#define PROC_MANAGEMENT_H

#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "utility.h"
#include "graph.h"
#include "parser.h"

pthread_mutex_t lock;

struct thread_arg {
    struct node *graph;
    int id;
};

int parent_done(struct node *graph, int id); 
void execproc(struct node *proc); 
void mutex_unlock(); 
void mutex_lock(); 
void *proc_routine(void *arg); 
void startproc(struct node *graph); 

#endif
