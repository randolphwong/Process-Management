/*
 * TODO:
 *  check status of waitpid
 *  if status is not exit success, then exit failure
 *  
 */

#include "proc_management.h"

int parent_done(struct node *graph, int id) {
    int i;
    for (i = 0; i < graph[id].parent_count; i++) {
        if (graph[graph[id].parent[i]].status != FINISHED) return 0;
    }
    return 1;
}

void execproc(struct node *proc) {
    struct Command *cmd;
    char *args[MAX_CHILDREN * 256];
    int i = 0;

    printf("executing process %d\n", proc->id);
    if ((strcmp(proc->input, "stdin")) != 0) {
        if ((close(fileno(stdin))) < 0)
            handle_error("close");
        if ((open(proc->input, O_RDONLY)) < 0)
            handle_error("open");
    }

    if ((strcmp(proc->output, "stdout")) != 0) {
        if ((close(fileno(stdout))) < 0)
            handle_error("close");
        if ((open(proc->output, O_WRONLY | O_CREAT | O_TRUNC, 
                        S_IRWXU | S_IRWXG | S_IROTH)) < 0)
            handle_error("open");
    }

    cmd = makecommand(proc->prog);
    if (!cmd) {
        fprintf(stderr, "Invalid program arguments.\n");
        fprintf(stderr, "Process %d will be terminated.\n", proc->id);
        exit(EXIT_FAILURE);
    }

    while ((args[i++] = nextarg(cmd)));

    if ((execvp(args[0], args)) < 0) {
        fprintf(stderr, "failed to execute process %d\n", proc->id);
        perror("execlp");
    }
}

void mutex_unlock() {
    int s;
    s = pthread_mutex_unlock(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_unlock");
}

void mutex_lock() {
    int s;
    s = pthread_mutex_lock(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_lock");
}

void *proc_routine(void *arg) {
    struct node *graph = ((struct thread_arg *)arg)->graph;
    struct node *proc = &graph[((struct thread_arg *)arg)->id];
    struct node *child;
    struct thread_arg child_args[proc->child_count];
    int s, i;
    int status;
    int child_ready;
    int exec_count = 0;
    pid_t pid;
    pthread_t proc_threads[proc->child_count];

    if ((waitpid(proc->pid, &status, 0)) < 0)
        handle_error("waitpid");

    mutex_lock();
    proc->status = FINISHED;
    mutex_unlock();

    for (i = 0; i < proc->child_count; i++) {
        child_ready = 0;
        child = &graph[proc->children[i]];

        mutex_lock();
        if (parent_done(graph, child->id) && (child->status == INELIGIBLE)) {
            child->status = READY;
            child_ready = 1;
        }
        mutex_unlock();

        if (!child_ready) continue;

        if ((pid = fork()) < 0) {
            handle_error("fork");
        } else if (pid == 0) {
            execproc(child);
        }

        mutex_lock();
        child->pid = pid;
        child->status = RUNNING;
        mutex_unlock();

        child_args[i].graph = graph;
        child_args[i].id = child->id;
        s = pthread_create(&proc_threads[exec_count++], NULL, proc_routine,
                &child_args[i]);
        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    for (i = 0; i < exec_count; i++) {
        s = pthread_join(proc_threads[i], NULL);
        if (s != 0)
            handle_error_en(s, "pthread_join");
    }

    return NULL;
}

void startproc(struct node *graph) {
    int s;
    pid_t pid;
    pthread_t proc_thread;
    struct thread_arg arg;

    /*
     * assumes that process 0 is READY by default
     */
    if ((pid = fork()) < 0) {
        handle_error("fork");
    } else if (pid == 0) {
        execproc(&graph[0]);
    }

    graph[0].pid = pid;
    graph[0].status = RUNNING;

    s = pthread_mutex_init(&lock, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_init");

    arg.graph = graph;
    arg.id = 0;
    s = pthread_create(&proc_thread, NULL, proc_routine, &arg);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    s = pthread_join(proc_thread, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");

    s = pthread_mutex_destroy(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_destroy");
}

