/*
 * Sequential Prim's Algorithm
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

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

    weight_t min_weight, g_min_weight;
    int min_node, g_min_node, min_node_connection, g_min_connection;
    int i, c;

    // Instantiate local section of g
    weight_t **myG = new weight_t *[v];
    myG[0] = new weight_t[v * perProcess];
    for (i = 1; i < perProcess; i++) myG[i] = myG[i - 1] + v;

    for (i = 1; i < v; i++) {
        in_mst[i] = false;
    }

    MPI_Scatter(g[0], v * perProcess, MPI_WEIGHT_TYPE, myG[0], v * perProcess, MPI_WEIGHT_TYPE, 0, MPI_COMM_WORLD);

    // Initialize d and e
    for (i = 0; i < perProcess; i++) {
        if (myG[i][0] != NO_EDGE) {
            d[i] = myG[i][0];
            e[i] = 0;
        } else {
            d[i] = NO_EDGE;
            e[i] = -1;
        }
    }

    for (c = 1; c < v; c++) {
        min_node = v + 1;
        min_node_connection = v + 1;
        min_weight = MAX_WEIGHT + 1;

        for (i = 0; i < perProcess; i++) {
            if (!in_mst[i + myStart] && d[i] < min_weight) {
                min_node = i + myStart;
                min_weight = d[i];
                min_node_connection = e[i];
            }
        }

        cout << myProcessId << " Found " << min_node << "<-" << min_weight << "->" << min_node_connection << endl;

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

        if (myProcessId == 0) {
            cout << "Min reduction " << g_min_node << "<-" << g_min_weight << "->" << g_min_connection << endl;
        }
        in_mst[g_min_node] = true;
        if (myProcessId == 0) {
            mst[g_min_connection][g_min_node] = g[g_min_connection][g_min_node];
            mst[g_min_node][g_min_connection] = g[g_min_connection][g_min_node];
        }

        for (i = 0; i < perProcess; i++) {
            if (!in_mst[i + myStart] && myG[i][g_min_node] < d[i]) {
                d[i] = myG[i][g_min_node];
                e[i] = g_min_node;
            }
        }
    }
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
    }

    adjacency_matrix_prims(g, mst, args.v, numProcesses, myProcessId);

    if (myProcessId == 0) {
        gettimeofday(&end, NULL);

        runtime = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;

        cout << "MPI prim's on " << args.v << " vertices runs in " << setiosflags(ios::fixed) << setprecision(3)
             << runtime << " seconds\n";

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

    MPI_Finalize();
}