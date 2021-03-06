/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef short weight_t;

#include "InitializeGraph.h"
#include "DotGen.h"
#include "Arguments.h"
#include "Print.h"

using namespace std;


void adjacency_matrix_prims(weight_t **g, weight_t **mst, const int v) {
    int mst_count = 1;
    bool *in_mst = new bool[v];
    in_mst[0] = true;
    weight_t *d = new weight_t[v];
    int *e = new int[v];
    weight_t min_weight, my_min_weight;
    int min_node, min_node_connection, my_min_node, my_min_connection;
    int i, c;
    // Locks stuff
    int myId, myLockId;
    weight_t *min_weights;
    int *min_connections, *min_nodes;

    int lg = (int) floor(log2f((float) omp_get_num_procs())) + 1;
    omp_lock_t *locks = new omp_lock_t[lg];
    min_connections = new int[lg];
    min_nodes = new int[lg];
    min_weights = new weight_t[lg];

    for (i = 0; i < lg; i++) {
        omp_init_lock(&locks[i]);
    }

#pragma omp parallel shared(d, e, g, mst, in_mst, min_weight, min_weights, min_node, min_node_connection, lg) private(i, c, my_min_weight, my_min_node, my_min_connection, myId)
    {
        myId = omp_get_thread_num();
        myLockId = (int) floor(log2f((float) myId));
        // Initialize d and e
#pragma omp for schedule(static)
        for (i = 1; i < v; i++) {
            in_mst[i] = false;
            if (g[0][i] != NO_EDGE) {
                d[i] = g[0][i];
                e[i] = 0;
            } else {
                d[i] = NO_EDGE;
                e[i] = -1;
            }
        }

        for (c = 1; c < v; c++) {
#pragma omp single
            {
                min_node = -1;
                min_node_connection = -1;
                min_weight = MAX_WEIGHT + 1;
            };
#pragma omp for
            for (i = 0; i < lg; i++) {
                min_nodes[i] = -1;
                min_connections[i] = -1;
                min_weights[i] = MAX_WEIGHT + 1;
            }
#pragma omp barrier

            my_min_weight = MAX_WEIGHT + 1;
            my_min_node = -1;
            my_min_connection = -1;

            // Find my minimum
#pragma omp for schedule(static)
            for (i = 0; i < v; i++) {
                if (!in_mst[i] && d[i] < my_min_weight) {
                    my_min_node = i;
                    my_min_weight = d[i];
                    my_min_connection = e[i];
                }
            }

            // Use myLockId to update minimum weight if applicable
            // Multiple locations behind locks allow parallel reduction to a limited set, which can be done more quickly afterwords
            if (my_min_weight < min_weights[myLockId]) {
                omp_set_lock(&locks[myLockId]);
                if (my_min_weight < min_weights[myLockId]) {
                    min_nodes[myLockId] = my_min_node;
                    min_weights[myLockId] = my_min_weight;
                    min_connections[myLockId] = my_min_connection;
                }
                omp_unset_lock(&locks[myLockId]);
            }
#pragma omp barrier

            // Find minimum and update in_mst marker and add edge to MST
#pragma omp single
            {
                for (i = 0; i < lg; i++) {
                    if (min_weights[i] < min_weight) {
                        min_weight = min_weights[i];
                        min_node = min_nodes[i];
                        min_node_connection = min_connections[i];
                    }
                }
                in_mst[min_node] = true;
                mst[min_node][min_node_connection] = g[min_node][min_node_connection];
                mst[min_node_connection][min_node] = g[min_node][min_node_connection];
            };
#pragma omp barrier

            // Update candidate edges
#pragma omp for
            for (i = 0; i < v; i++) {
                if (!in_mst[i] && g[min_node][i] < d[i]) {
                    d[i] = g[min_node][i];
                    e[i] = min_node;
                }
            }
#pragma omp barrier
        }
    };
}

int main(int argc, char *argv[]) {
    Arguments args;

    args = ParseArguments(argc, argv);
    if (args.error) {
        exit(1);
    }

    // Instantiate g and mst
    weight_t **g, **mst;
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