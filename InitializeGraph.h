//
// Created by Alex on 11/3/2017.
//

#ifndef MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
#define MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H

#include <stdlib.h>
#include <math.h>

using namespace std;

#define MAX_WEIGHT 500
#define NO_EDGE MAX_WEIGHT+1

//#include "Graph.h"


/********************
 * Default settings *
 ********************/
struct GraphGenParams {
    // pEdgeAdd/pEdgeIn = probably of adding each edge
    int pEdgeAdd;
    int pEdgeIn;

    int maxWeight;

    GraphGenParams() : pEdgeAdd(1), pEdgeIn(1000), maxWeight(MAX_WEIGHT) {}

    bool includeNext() {
        return rand() % pEdgeIn < pEdgeAdd;
    }

    short randWeight() {
        return rand() % (2 * maxWeight) - maxWeight;
    }

    void fillNoEdge(short **&g, int v) {
        for (int i = 0; i < v; i++) {
            for (int j = 0; j <= i; j++) {
                g[i][j] = g[j][i] = NO_EDGE;
            }
        }
    }

    /**
     * Initialize the graph
     * @param g
     * @param v
     */
    void initializeGraph(short **&g, int v) {
        g = new short *[v];
        g[0] = new short[(long) v * (long) v];
        for (int i = 1; i < v; i++) {
            g[i] = g[i - 1] + v;
        }

        if (pEdgeAdd == 0) {
            fillNoEdge(g, v);
            return;
        }

        for (int i = 0; i < v; i++) {
            for (int j = 0; j <= i; j++) {
                if (i != j && includeNext()) {
                    g[i][j] = g[j][i] = randWeight();
                } else {
                    g[i][j] = g[j][i] = NO_EDGE;
                }
            }
        }

        // Make sure there's at least one edge on each vertex
//        bool hasEdge = false;
//        int newEdge = 0;
//        for (int i = 0; i < v; i++) {
//            hasEdge = false;
//            for (int j = 0; j < v; j++) {
//                if (i != j && g[i][j] != NO_EDGE) {
//                    hasEdge = true;
//                    break;
//                }
//            }
//
//            if (!hasEdge) {
//                // None found, add one to a random other vertex
//                newEdge = rand() % v;
//                while (newEdge == i) {
//                    newEdge = rand() % v;
//                }
//                g[i][newEdge] = g[newEdge][i] = randWeight();
//            }
//        }
    }

//    void initializeGraph(Graph g) {
//        for (int i = 0; i < g.length(); i++) {
//            for (int j = 0; j < i; j++) {
//                if (includeNext()) {
//                    g.addEdge(i, j, randWeight());
//                }
//            }
//        }
//
//        // Make sure there's at least one edge on each vertex
//        bool hasEdge = false;
//        int newEdge = 0;
//        for (int i = 0; i < g.length(); i++) {
//            hasEdge = false;
//            for (int j = 0; j < g.length(); j++) {
//                if (g.weight(i, j) != NO_EDGE) {
//                    hasEdge = true;
//                    break;
//                }
//            }
//
//            if (!hasEdge) {
//                // None found, add one to a random other vertex
//                newEdge = rand() % g.length();
//                while (newEdge == i) {
//                    newEdge = rand() % g.length();
//                }
//
//                g.addEdge(i, newEdge, randWeight());
//            }
//        }
//    }
};

GraphGenParams noEdges(int v) {
    GraphGenParams g = GraphGenParams();
    g.pEdgeAdd = 0;

    return g;
}

GraphGenParams graphGenDense(int v) {
    GraphGenParams g = GraphGenParams();

    // Connect to ~95%
    g.pEdgeAdd = ceil(0.95 * v);
    g.pEdgeIn = v;

    return g;
}

GraphGenParams graphGenSparse(int v) {
    GraphGenParams g = GraphGenParams();

    // Connect to ~5%
    g.pEdgeAdd = ceil(0.1 * v);
    g.pEdgeIn = v;

    return g;
}

void copy(int **source, int **destination, int v) {
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

#endif //MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
