# Process-Management

Done by: Chiew Jun Hao 1001046, Wong Wai Kit Randolph 1001043

## Purpose
- To develop a processmgt.c / processmgt.java program by using processes (fork, exec,
  wait, system) in C or (ProcessBuilder) in java to execute a graph of user programs in
  parallel
- Graph of user programs are executed by reading commands in text file
- To model control and data dependencies
- Control dependence: a program cannot start until its predecessor(s) is finished
- Data dependences: a program requires input from its predecessor(s) before it can
  execute

## What it does & how it works

The program is split into two parts: file parsing (file\_parser.c) and process
management (proc\_management.c). A data structure (struct node) is used to
represent each process.

### File parsing

File provided in the argument is read line by line. For each line, the program
will convert the corresponding tokens delimited by colons into a struct node.
This is done using strtok.

The file parser will return a struct graph that contains an array of nodes and
roots. The graph will be checked for cycles and any potential invalid input (see
section on Behaviour).

### Process management

This part begins by forking new processes for all the roots (since they are
READY by default) and then executing their programs. Next, for each new
processes created, a corresponding thread will be created to wait for it. This
allows for true parallelism among the processes.

After waiting for the process, each thread will in turn fork new processes for
all the children of that node and execute the program. This thread recursively
creates new thread to wait for each of the child processes.

The execution involves redirection (if necessary) of the input and output
using dup2. Finally, the program is executed with execvp.

## Behaviour
1. What happens when the graph has a cycle?

   The program will print an error. No cycles are allowed.

2. What happens when the graph has no roots (node without parents)?

   The program will print an error.

3. What happens when file contains nothing?

   The program will print an error.

4. If the next line is only whitespace, what happens?

   Empty lines are ignored.

5. What happens if there is a missing colon?

   The program will print an error.

6. What happens if there is a blank token (whitespaces)?

   The program will print an error.

7. What happens when the children is a number greater than the total number of
  lines?

   The program will print an error.

8. What happens when children-list token is not a number?

   The program will print an error.

## How to compile and run
1. run `make`
2. run `./bin/processmgt filename`

