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

#pragma omp parallel shared(d, e, g, mst, in_mst, min_weight, min_node, min_node_connection) private(i, c, my_min_weight, my_min_node, my_min_connection)
    {
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

            my_min_weight = MAX_WEIGHT + 1;
            my_min_node = -1;
            my_min_connection = -1;
#pragma omp barrier

#pragma omp for
            for (i = 0; i < v; i++) {
                if (!in_mst[i] && d[i] < my_min_weight) {
                    my_min_node = i;
                    my_min_weight = d[i];
                    my_min_connection = e[i];
                }
            }
#pragma omp critical
            {
                if (my_min_weight < min_weight) {
                    min_node = my_min_node;
                    min_weight = my_min_weight;
                    min_node_connection = my_min_connection;
                }
            };
#pragma omp barrier

#pragma omp single
            {
                in_mst[min_node] = true;
                if (min_node < min_node_connection)
                    mst[min_node][min_node_connection] = g[min_node][min_node_connection];
                else
                    mst[min_node_connection][min_node] = g[min_node_connection][min_node];
            };
#pragma omp barrier

#pragma omp for
            for (i = 0; i < v; i++) {
                if (!in_mst[i] && g[min_node][i] < d[i]) {
                    d[i] = g[min_node][i];
                    e[i] = min_node;
                }
            }
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