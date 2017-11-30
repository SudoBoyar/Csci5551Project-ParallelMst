/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define VCOUNT 1000


#include "InitializeGraph.h"
#include "DotGen.h"


using namespace std;

void printMatrix(int **a, int n) {
    for (int i = 0; i < n; i++) {
        cout << "Row " << (i + 1) << ":\t";
        for (int j = 0; j < n; j++) {
            printf("%d\t", a[i][j]);
        }
        cout << endl;
    }
}

void adjacency_matrix_prims(int **g, int **mst, const int v) {
    int mst_count = 1;
    bool *in_mst = new bool[v];
    in_mst[0] = true;
    int *d = new int[v];
    int *e = new int[v];

    // Initialize d and e
    for (int i = 0; i < v; i++) {
        if (g[0][i] != NO_EDGE) {
            d[i] = g[0][i];
            e[i] = 0;
        } else {
            d[i] = NO_EDGE;
            e[i] = -1;
        }
    }

    int min_weight, min_node, min_node_connection;

    for (int c = 1; c < v; c++) {
        min_node = -1;
        min_node_connection = -1;
        min_weight = MAX_WEIGHT + 1;

        for (int i = 0; i < v; i++) {
            if (in_mst[i] || d[i] == NO_EDGE || d[i] >= min_weight) {
                // Already in MST, no edge to consider, or we already found a better one
                continue;
            }

            min_node = i;
            min_weight = d[i];
            min_node_connection = e[i];
        }

        // TODO Fix generation so no disconnected components
        if (min_node > -1) {
//            printf("Adding %d to MST at %d with weight %d\n", min_node, min_node_connection, min_weight);

            in_mst[min_node] = true;
            mst[min_node_connection][min_node] = g[min_node_connection][min_node];
            mst[min_node][min_node_connection] = g[min_node_connection][min_node];

            for (int j = 0; j < v; j++) {
                if (in_mst[j]) {
                    continue;
                }

                if ((d[j] == NO_EDGE && g[min_node][j] != NO_EDGE) || (d[j] != NO_EDGE && g[min_node][j] != NO_EDGE && g[min_node][j] < d[j])) {
                    d[j] = g[min_node][j];
                    e[j] = min_node;
                }
            }
        }
    }
}


int main() {
    int v = 100000;

    // Instantiate g and mst
    int **g, **mst;
    float start, end, runtime;

    // Initialize g and mst
    printf("Initializing Graph\n");
    GraphGenParams graph = graphGenDense(v);
    graph.initializeGraph(g, v);

    printf("Initializing MST\n");
    GraphGenParams empty = noEdges(v);
    empty.initializeGraph(mst, v);

    // Run it
    printf("Running Prim's\n");
    start = clock()/(float)CLOCKS_PER_SEC;
    adjacency_matrix_prims(g, mst, v);
    end = clock()/(float)CLOCKS_PER_SEC;

    runtime = end - start;

    cout<< "Sequential runs in " << setiosflags(ios::fixed) << setprecision(3) << runtime << " seconds\n";

//    char *filename = new char[100];
//    // Output input graph
//    snprintf(filename, 100, "results/sequential_sparse_%d_input.dot", v);
//    matrixOutput(g, v, filename);
//
//    // Output mst
//    snprintf(filename, 100, "results/sequential_sparse_%d_result.dot", v);
//    matrixOutput(mst, v, filename);
//
//    // Output mst highlighted in input graph
//    snprintf(filename, 100, "results/sequential_sparse_%d_mst_overlay.dot", v);
//    matrixMstOutput(g, mst, v, filename);
}