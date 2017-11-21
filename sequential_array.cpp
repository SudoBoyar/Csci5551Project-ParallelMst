/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "InitializeGraph.h
#include "DotGen.h"

using namespace std;

int main() {
    int **g;
    int v = 10000;

    GraphGenParams sparse = graphGenSparse(v);
    GraphGenParams dense = graphGenDense(v);

    sparse.initializeGraph(g, v);
    matrixOutput(g, v, "sparse_10k_test.dot");
}