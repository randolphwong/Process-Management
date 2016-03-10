/*
 * Programming Assignment 1
 * Author   : Chiew Jun Hao (1001046)
 * Author   : Wong Wai Kit Randolph (1001043)
 * Date     : 10/03/2016
 */

#include <stdio.h>

#include "graph.h"
#include "proc_management.h"

int main(void) {
    struct node graph[5];
    graph[0].id = 0;
    graph[0].parent_count = 0;
    graph[0].children[0] = 1;
    graph[0].child_count = 1;
    graph[0].status = READY;
    strcpy(graph[0].prog, "echo hi there");
    strcpy(graph[0].input, "stdin");
    strcpy(graph[0].output, "echo-out.txt");

    graph[1].id = 1;
    graph[1].parent[0] = 0;
    graph[1].parent_count = 1;
    graph[1].children[0] = 2;
    graph[1].children[1] = 3;
    graph[1].child_count = 2;
    graph[1].status = INELIGIBLE;
    strcpy(graph[1].prog, "echo wazzup");
    strcpy(graph[1].input, "stdin");
    strcpy(graph[1].output, "echo2-out.txt");

    graph[2].id = 2;
    graph[2].parent[0] = 1;
    graph[2].parent_count = 1;
    graph[2].children[0] = 4;
    graph[2].child_count = 1;
    graph[2].status = INELIGIBLE;
    strcpy(graph[2].prog, "cat");
    strcpy(graph[2].input, "echo-out.txt");
    strcpy(graph[2].output, "cat-1.txt");

    graph[3].id = 3;
    graph[3].parent[0] = 1;
    graph[3].parent_count = 1;
    graph[3].children[0] = 4;
    graph[3].child_count = 1;
    graph[3].status = INELIGIBLE;
    strcpy(graph[3].prog, "cat");
    strcpy(graph[3].input, "echo2-out.txt");
    strcpy(graph[3].output, "cat-2.txt");

    graph[4].id = 4;
    graph[4].parent[0] = 2;
    graph[4].parent[1] = 3;
    graph[4].parent_count = 2;
    graph[4].child_count = 0;
    graph[4].status = INELIGIBLE;
    strcpy(graph[4].prog, "cat cat-1.txt cat-2.txt");
    strcpy(graph[4].input, "stdin");
    strcpy(graph[4].output, "sink.txt");


    /*struct node graph[11];*/
    /*graph[0].id = 0;*/
    /*graph[0].parent_count = 0;*/
    /*graph[0].children[0] = 1;*/
    /*graph[0].child_count = 1;*/
    /*graph[0].status = READY;*/

    /*graph[1].id = 1;*/
    /*graph[1].parent[0] = 0;*/
    /*graph[1].parent_count = 1;*/
    /*graph[1].children[0] = 2;*/
    /*graph[1].children[1] = 3;*/
    /*graph[1].child_count = 2;*/
    /*graph[1].status = INELIGIBLE;*/

    /*graph[2].id = 2;*/
    /*graph[2].parent[0] = 1;*/
    /*graph[2].parent_count = 1;*/
    /*graph[2].children[0] = 4;*/
    /*graph[2].children[1] = 5;*/
    /*graph[2].child_count = 2;*/
    /*graph[2].status = INELIGIBLE;*/

    /*graph[3].id = 3;*/
    /*graph[3].parent[0] = 1;*/
    /*graph[3].parent_count = 1;*/
    /*graph[3].children[0] = 6;*/
    /*graph[3].children[1] = 7;*/
    /*graph[3].children[2] = 8;*/
    /*graph[3].child_count = 3;*/
    /*graph[3].status = INELIGIBLE;*/

    /*graph[4].id = 4;*/
    /*graph[4].parent[0] = 2;*/
    /*graph[4].parent_count = 1;*/
    /*graph[4].children[0] = 9;*/
    /*graph[4].child_count = 1;*/
    /*graph[4].status = INELIGIBLE;*/

    /*graph[5].id = 5;*/
    /*graph[5].parent[0] = 2;*/
    /*graph[5].parent_count = 1;*/
    /*graph[5].children[0] = 10;*/
    /*graph[5].child_count = 1;*/
    /*graph[5].status = INELIGIBLE;*/

    /*graph[6].id = 6;*/
    /*graph[6].parent[0] = 3;*/
    /*graph[6].parent_count = 1;*/
    /*graph[6].children[0] = 10;*/
    /*graph[6].child_count = 1;*/
    /*graph[6].status = INELIGIBLE;*/

    /*graph[7].id = 7;*/
    /*graph[7].parent[0] = 3;*/
    /*graph[7].parent_count = 1;*/
    /*graph[7].children[0] = 10;*/
    /*graph[7].child_count = 1;*/
    /*graph[7].status = INELIGIBLE;*/

    /*graph[8].id = 8;*/
    /*graph[8].parent[0] = 3;*/
    /*graph[8].parent_count = 1;*/
    /*graph[8].children[0] = 10;*/
    /*graph[8].child_count = 1;*/
    /*graph[8].status = INELIGIBLE;*/

    /*graph[9].id = 9;*/
    /*graph[9].parent[0] = 4;*/
    /*graph[9].parent_count = 1;*/
    /*graph[9].children[0] = 10;*/
    /*graph[9].child_count = 1;*/
    /*graph[9].status = INELIGIBLE;*/

    /*graph[10].id = 10;*/
    /*graph[10].parent[0] = 9;*/
    /*graph[10].parent[1] = 5;*/
    /*graph[10].parent[2] = 6;*/
    /*graph[10].parent[3] = 6;*/
    /*graph[10].parent[4] = 8;*/
    /*graph[10].parent_count = 5;*/
    /*graph[10].child_count = 0;*/
    /*graph[10].status = INELIGIBLE;*/

    startproc(graph);

    return 0;
}
