/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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

void adjacency_matrix_prims(short **g, short **mst, const int v) {
    int mst_count = 1;
    bool *in_mst = new bool[v];
    in_mst[0] = true;
    short *d = new short[v];
    int *e = new int[v];
    short min_weight;
    int min_node, min_node_connection;

#pragma omp parallel shared(d, e, g, mst, in_mst, min_weight, min_node, min_node_connection)
    {
        // Initialize d and e
#pragma omp for
        for (int i = 0; i < v; i++) {
            if (g[0][i] != NO_EDGE) {
                d[i] = g[0][i];
                e[i] = 0;
            } else {
                d[i] = NO_EDGE;
                e[i] = -1;
            }
        }


        for (int c = 1; c < v; c++) {
#pragma omp single
            {
                min_node = -1;
                min_node_connection = -1;
                min_weight = MAX_WEIGHT + 1;
            }

#pragma omp for
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
//                printf("Adding %d to MST at %d with weight %d\n", min_node, min_node_connection, min_weight);

#pragma omp single
                {
                    in_mst[min_node] = true;
                    mst[min_node_connection][min_node] = g[min_node_connection][min_node];
                    mst[min_node][min_node_connection] = g[min_node_connection][min_node];
                }

#pragma omp for
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
}


bool GetUserInput(int argc, char *argv[], int &v, bool &verbose) {
    if (argc < 2) {
        v = 100000;
    } else {
        v = atoi(argv[1]);
        if (v <= 0) {
            cout << "Graphs need vertices" << endl;
            return false;
        }

        if (argc >= 3) {
            verbose = true;
        } else {
            verbose = false;
        }
    }

    return true;
}


int main(int argc, char *argv[]) {
    int v;
    bool verbose;

    if (!GetUserInput(argc, argv, v, verbose)) {
        exit(1);
    }

    // Instantiate g and mst
    short **g, **mst;
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
    start = omp_get_wtime();
    adjacency_matrix_prims(g, mst, v);
    end = omp_get_wtime();

    runtime = end - start;

    cout<< v << " vertices OpenMP on " << omp_get_num_procs() << " threads/CPUs runs in " << setiosflags(ios::fixed) << setprecision(3) << runtime << " seconds\n";

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