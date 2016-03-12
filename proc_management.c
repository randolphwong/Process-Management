#include "proc_management.h"

/**
 * parent_done - checks the status of all the parent of the given node
 *
 * Returns 1 if all parent's status is FINISHED, and 0 otherwise.
 */
int parent_done(struct graph *g, int id)
{
    int i;
    int parent;
    for (i = 0; i < g->procs[id]->parent_count; i++) {
        parent = g->procs[id]->parent[i];
        if (g->procs[parent]->status != FINISHED) return 0;
    }
    return 1;
}

/**
 * execproc - opens the input and output file for the corresponding program and
 * executes it with execvp
 */
void execproc(struct node *proc)
{
    struct Command *cmd;
    char *args[MAX_CHILDREN * 256];
    int tmp_fd;
    int i = 0;

    printf("executing process %d\n", proc->id);
    if ((strcmp(proc->input, "stdin")) != 0) {
        if ((tmp_fd = open(proc->input, O_RDONLY)) < 0)
            handle_error("open");
        if ((dup2(tmp_fd, fileno(stdin))) < 0)
            handle_error("dup2");
    }

    if ((strcmp(proc->output, "stdout")) != 0) {
        if ((tmp_fd = open(proc->output, O_WRONLY | O_CREAT | O_TRUNC, 
                        S_IRWXU | S_IRWXG | S_IROTH)) < 0)
            handle_error("open");
        if ((dup2(tmp_fd, fileno(stdout))) < 0)
            handle_error("dup2");
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

void mutex_unlock()
{
    int s;
    s = pthread_mutex_unlock(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_unlock");
}

void mutex_lock()
{
    int s;
    s = pthread_mutex_lock(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_lock");
}

/**
 * check_status - checks the status that is updated by wait(2)
 *
 * Returns 0 on a normal exit by the program, and -1 otherwise.
 */
int check_status(int *status)
{
    int stat = -1;

    if (WIFEXITED(*status))
        stat = WEXITSTATUS(*status);

    return stat;
}

/* used as argument for proc_routine */
struct thread_arg {
    struct graph *proc_graph;
    int id;
};

/**
 * proc_routine - a thread routine that:
 * 1. waits for the specified process (in the argument)
 * 2. create the processes for all children that is ready and then
 * 3. recursively create new threads (proc_routine) to wait for the child.
 */
void *proc_routine(void *arg)
{
    struct graph *proc_graph = ((struct thread_arg *)arg)->proc_graph;
    struct node *proc = proc_graph->procs[((struct thread_arg *)arg)->id];
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

    if ((check_status(&status)) != 0) {
        fprintf(stderr, "process %d terminated abnormally.\n", proc->id);
        exit(EXIT_FAILURE);
    }

    mutex_lock();
    proc->status = FINISHED;
    mutex_unlock();

    for (i = 0; i < proc->child_count; i++) {
        child_ready = 0;
        child = proc_graph->procs[proc->children[i]];

        mutex_lock();
        if (parent_done(proc_graph, child->id) &&
                (child->status == INELIGIBLE)) {
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

        child_args[i].proc_graph = proc_graph;
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

/**
 * startproc - create a new process for node 0 and create a new thread
 * (proc_routine) to wait for it.
 */
void startproc(struct graph *proc_graph)
{
    int s, i;
    pid_t pid;
    pthread_t proc_threads[proc_graph->root_count];
    struct node *root;
    struct thread_arg arg;

    s = pthread_mutex_init(&lock, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_init");

    for (i = 0; i < proc_graph->root_count; i++) {
        root = proc_graph->roots[i];

        if ((pid = fork()) < 0) {
            handle_error("fork");
        } else if (pid == 0) {
            execproc(proc_graph->roots[i]);
        }

        root->pid = pid;
        root->status = RUNNING;

        arg.proc_graph = proc_graph;
        arg.id = root->id;
        s = pthread_create(proc_threads + i, NULL, proc_routine, &arg);
        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    for (i = 0; i < proc_graph->root_count; i++) {
        s = pthread_join(proc_threads[i], NULL);
        if (s != 0)
            handle_error_en(s, "pthread_join");
    }

    s = pthread_mutex_destroy(&lock);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_destroy");
}

