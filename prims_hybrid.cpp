/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef short weight_t;
#define MPI_WEIGHT_TYPE MPI_SHORT

#include "InitializeGraph.h"
#include "DotGen.h"
#include "Arguments.h"
#include "Print.h"


using namespace std;


void adjacency_matrix_prims(weight_t **g, weight_t **mst, const int v, int numProcesses, int myProcessId) {
    int perProcess = v / numProcesses;
    int myStart = myProcessId * perProcess;
    int myEnd = (myProcessId + 1) * perProcess;
    if (myEnd > v) {
        myEnd = v;
    }

    bool *in_mst = new bool[v];
    in_mst[0] = true;
    weight_t *d = new weight_t[perProcess];
    int *e = new int[perProcess];

    weight_t min_weight, g_min_weight, my_min_weight;
    int min_node, g_min_node, min_node_connection, g_min_connection, my_min_node, my_min_connection;
    int i, c;

//    int myId, myLockId;
//    weight_t *min_weights;
//    int *min_connections, *min_nodes;
//
//    int lg = (int) floor(log2f((float) omp_get_num_procs()));
//    omp_lock_t *locks = new omp_lock_t[lg];
//    min_connections = new int[lg];
//    min_nodes = new int[lg];
//    min_weights = new weight_t[lg];
//
//    for (i = 0; i < lg; i++) {
//        omp_init_lock(&locks[i]);
//    }

    for (int i = 1; i < v; i++) {
        in_mst[i] = false;
    }

    // Instantiate local section of g
    weight_t **myG = new weight_t *[perProcess];
    myG[0] = new weight_t[v * perProcess];
    for (int i = 1; i < perProcess; i++) {
        myG[i] = myG[i - 1] + v;
    }

    MPI_Scatter(g[0], v * perProcess, MPI_WEIGHT_TYPE, myG[0], v * perProcess, MPI_WEIGHT_TYPE, 0, MPI_COMM_WORLD);

//#pragma omp parallel shared(d, e, g, myG, mst, in_mst, min_weight, min_node, min_node_connection, g_min_node, g_min_connection, g_min_weight, min_nodes, min_weights, min_connections, lg) private(i, c, my_min_weight, my_min_node, my_min_connection, myId, myLockId)
#pragma omp parallel shared(d, e, g, myG, mst, in_mst, min_weight, min_node, min_node_connection, g_min_node, g_min_connection, g_min_weight) private(i, c, my_min_weight, my_min_node, my_min_connection)
    {
//        myId = omp_get_thread_num();
//        myLockId = (int) floor(log2f((float) myId));

        // Initialize d and e
#pragma omp for schedule(static)
        for (i = 0; i < perProcess; i++) {
            if (myG[i][0] != NO_EDGE) {
                d[i] = myG[i][0];
                e[i] = 0;
            } else {
                d[i] = NO_EDGE;
                e[i] = v + 1;
            }
        }

        for (c = 1; c < v; c++) {
#pragma omp single
            {
                min_node = -1;
                min_node_connection = -1;
                min_weight = MAX_WEIGHT + 1;
            };
//#pragma omp for
//            for (i = 0; i < lg; i++) {
//                min_nodes[i] = -1;
//                min_connections[i] = -1;
//                min_weights[i] = MAX_WEIGHT + 1;
//            }
//#pragma omp barrier

            my_min_node = v + 1;
            my_min_connection = v + 1;
            my_min_weight = MAX_WEIGHT + 1;

            // Find OpenMP local minimum
#pragma omp for schedule(static)
            for (i = 0; i < perProcess; i++) {
                if (!in_mst[i + myStart] && d[i] < my_min_weight) {
                    my_min_node = i + myStart;
                    my_min_weight = d[i];
                    my_min_connection = e[i];
                }
            }

//            if (my_min_weight < min_weights[myLockId]) {
//                omp_set_lock(&locks[myLockId]);
//                if (my_min_weight < min_weights[myLockId]) {
//                    min_nodes[myLockId] = my_min_node;
//                    min_weights[myLockId] = my_min_weight;
//                    min_connections[myLockId] = my_min_connection;
//                }
//                omp_unset_lock(&locks[myLockId]);
//            }

            // Find MPI process local minimum
#pragma omp critical
            {
                if (my_min_weight < min_weight) {
                    min_node = my_min_node;
                    min_weight = my_min_weight;
                    min_node_connection = my_min_connection;
                }
            };
#pragma omp barrier

            // MPI Reductions to find global minimum
#pragma omp single
            {
//                for (i = 0; i < lg; i++) {
//                    if (min_weights[i] < min_weight) {
//                        min_weight = min_weights[i];
//                        min_node = min_nodes[i];
//                        min_node_connection = min_connections[i];
//                    }
//                }

                // All reduce the min weight
                MPI_Allreduce(&min_weight, &g_min_weight, 1, MPI_WEIGHT_TYPE, MPI_MIN, MPI_COMM_WORLD);

                // All reduce the connecting node
                if (g_min_weight != min_weight) {
                    // Not mine, send invalid placeholder value
                    min_node_connection = v + 1;
                }
                MPI_Allreduce(&min_node_connection, &g_min_connection, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

                if (min_node_connection != g_min_connection) {
                    // Mine wasn't the min
                    min_node = v + 1;
                }

                MPI_Allreduce(&min_node, &g_min_node, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
            };
#pragma omp barrier

            // Update in_mst marker and add edge to MST
#pragma omp single
            {
                in_mst[g_min_node] = true;
                if (myProcessId == 0) {
                    mst[g_min_node][g_min_connection] = g[g_min_node][g_min_connection];
                    mst[g_min_connection][g_min_node] = g[g_min_node][g_min_connection];
                }
            };

            // Update candidate edges
#pragma omp for
            for (i = 0; i < perProcess; i++) {
                if (!in_mst[i + myStart] && myG[i][g_min_node] < d[i]) {
                    d[i] = myG[i][g_min_node];
                    e[i] = g_min_node;
                }
            }
#pragma omp barrier
        }
    };
}


int main(int argc, char *argv[]) {
    Arguments args;
    int numProcesses, myProcessId;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcessId);

    args = ParseArguments(argc, argv);
    if (args.error) {
        exit(1);
    }

    // Instantiate g and mst
    weight_t **g, **mst;
    float runtime;
    struct timeval start, end;

    if (myProcessId == 0) {
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
    } else {
        g = new weight_t *[1];
    }

    adjacency_matrix_prims(g, mst, args.v, numProcesses, myProcessId);

    if (myProcessId == 0) {
        gettimeofday(&end, NULL);

        runtime = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;

        cout << "Hybrid prim's on " << args.v << " vertices on " << numProcesses << " nodes each with "
             << omp_get_num_procs() << " threads/CPUs runs in " << setiosflags(ios::fixed) << setprecision(3) << runtime
             << " seconds\n";

        if (args.print) {
            cout << "Graph:" << endl << flush;
            Print2DMatrix(g, args.v);
            cout << "MST:" << endl << flush;
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

    MPI_Finalize();
}