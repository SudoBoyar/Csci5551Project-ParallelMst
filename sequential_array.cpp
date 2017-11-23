/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>


#define VCOUNT 100


#include "InitializeGraph.h"
#include "DotGen.h"


using namespace std;

void adjacency_matrix_prims(int **g, int **mst, const int v) {
    bool in_mst[VCOUNT] = {false};
    in_mst[1] = true;

    int min_weight, min_node, min_node_connection;
    for (int c = 1; c < v - 1; c++) {
        min_node = -1;
        min_node_connection = -1;
        min_weight = MAX_WEIGHT + 1;

        for (int i = 0; i < v; i++) {
            if (!in_mst[i]) {
                continue;
            }

            for (int j = 0; j < v; j++) {
                // Skip condition checks
                if (in_mst[j] || g[i][j] == NO_EDGE) {
                    // Already in MST (would make cycle) or no edge
                    continue;
                }

                // Check if min
                if (g[i][j] < min_weight) {
                    min_weight = g[i][j];
                    min_node = j;
                    min_node_connection = i;
                }
            }
        }

        // TODO Fix generation so no disconnected components
        if (min_node > -1) {
//            printf("Adding %d to MST at %d with weight %d\n", min_node, min_node_connection, min_weight);
            in_mst[min_node] = true;
            mst[min_node_connection][min_node] = g[min_node_connection][min_node];
            mst[min_node][min_node_connection] = g[min_node_connection][min_node];
        }
    }
}


int main() {
    char *filename = new char[100];
    int v = VCOUNT;
    printf("%d %d\n", v, VCOUNT);

    // Instantiate g and mst
    int **g, **mst;
    g = new int *[v];
    g[0] = new int[v * v];
    for (int i = 1; i < v; i++) {
        g[i] = g[i - 1] + v;
    }
    mst = new int *[v];
    mst[0] = new int[v * v];
    for (int i = 1; i < v; i++) {
        mst[i] = mst[i - 1] + v;
    }

    // Initialize g and mst
    GraphGenParams empty = noEdges(v);
    empty.initializeGraph(mst, v);
    GraphGenParams sparse = graphGenSparse(v);
    sparse.initializeGraph(g, v);

    adjacency_matrix_prims(g, mst, v);

    // Output input graph
    snprintf(filename, 100, "sparse_%d_input.dot", v);
    matrixOutput(g, v, filename);

    // Output mst
    snprintf(filename, 100, "sparse_%d_result.dot", v);
    matrixOutput(mst, v, filename);

    // Output mst highlighted in input graph
    snprintf(filename, 100, "sparse_%d_mst_overlay.dot", v);
    matrixMstOutput(g, mst, v, filename);
}