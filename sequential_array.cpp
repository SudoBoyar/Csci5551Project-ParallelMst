/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>

#include "InitializeGraph.h"
#include "DotGen.h"

#define VCOUNT 100

using namespace std;

int main() {
//    int v = VCOUNT;
    printf("%d\n", VCOUNT);
    int **g;
    g = new int *[VCOUNT];
    g[0] = new int[VCOUNT * VCOUNT];
    for (int i = 1; i < VCOUNT; i++) {
        g[i] = g[i - 1] + VCOUNT;
    }


    GraphGenParams sparse = graphGenSparse(VCOUNT);
    GraphGenParams dense = graphGenDense(VCOUNT);
    printf("Initializing\n");
    sparse.initializeGraph(g, VCOUNT);
    printf("Generated!\n");
    matrixOutput(g, VCOUNT, "sparse_100_test.dot");
}