/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#include "InitializeGraph.h"
#include "DotGen.h"
#include "Arguments.h"
#include "Print.h"

using namespace std;


void adjacency_matrix_prims(short **g, short **mst, const int v) {
    int mst_count = 1;
    bool *in_mst = new bool[v];
    in_mst[0] = true;
    short *d = new short[v];
    int *e = new int[v];
    short min_weight;
    int min_node, min_node_connection;

#pragma omp parallel shared(d, e, g, mst, in_mst, min_weight, min_node, min_node_connection) private(i, j, c)
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

#pragma omp barrier
#pragma omp for
            for (int i = 0; i < v; i++) {
                if (in_mst[i] || d[i] == NO_EDGE || d[i] >= min_weight) {
                    // Already in MST, no edge to consider, or we already found a better one
                    continue;
                }

#pragma omp critical
                {
                    min_node = i;
                    min_weight = d[i];
                    min_node_connection = e[i];
                }
            }

#pragma omp barrier

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

                    if ((d[j] == NO_EDGE && g[min_node][j] != NO_EDGE) ||
                        (d[j] != NO_EDGE && g[min_node][j] != NO_EDGE && g[min_node][j] < d[j])) {
                        d[j] = g[min_node][j];
                        e[j] = min_node;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    Arguments args;

    args = ParseArguments(argc, argv);
    if (args.error) {
        exit(1);
    }

    // Instantiate g and mst
    short **g, **mst;
    float runtime;
    struct timeval start, end;

    // Initialize g and mst
    printf("Initializing Graph\n");
    GraphGenParams graph = graphGenDense(args.v, args.seed);
    graph.initializeGraph(g, args.v);

    printf("Initializing MST\n");
    GraphGenParams empty = noEdges(args.v, args.seed);
    empty.initializeGraph(mst, args.v);

    // Run it
    printf("Running Prim's\n");
    gettimeofday(&start, NULL);
    adjacency_matrix_prims(g, mst, args.v);
    gettimeofday(&end, NULL);

    runtime = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;

    cout << args.v << " vertices OpenMP on " << omp_get_num_procs() << " threads/CPUs runs in "
         << setiosflags(ios::fixed)
         << setprecision(3) << runtime << " seconds\n";

    if (args.print) {
        cout << "Graph:" << endl;
        Print2DMatrix(g, args.v);
        cout << "MST:" << endl;
        Print2DMatrix(mst, args.v);
    }

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