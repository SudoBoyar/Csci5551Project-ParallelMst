//
// Created by Alex on 11/3/2017.
//

#ifndef MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
#define MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H

#include <stdlib.h>
#include <math.h>

using namespace std;

#define NO_EDGE -1001
#define MAX_WEIGHT 10

#include "Graph.h"


/********************
 * Default settings *
 ********************/
struct GraphGenParams {
    // pEdgeAdd/pEdgeIn = probably of adding each edge
    int pEdgeAdd = 1;
    int pEdgeIn = 1000;

    int maxWeight = MAX_WEIGHT;

    bool includeNext() {
        return rand() % pEdgeIn < pEdgeAdd;
    }

    int randWeight() {
        return rand() % (2 * maxWeight) - maxWeight;
    }

    /**
     * Initialize the graph
     * @param g
     * @param v
     */
    void initializeGraph(int **g, int v) {
        for (int i = 0; i < v; i++) {
            for (int j = 0; j < i; j++) {
                if (includeNext()) {
                    g[i][j] = g[j][i] = randWeight();
                } else {
                    g[i][j] = g[j][i] = NO_EDGE;
                }
            }
        }

        // Make sure there's at least one edge on each vertex
        bool hasEdge = false;
        int newEdge = 0;
        for (int i = 0; i < v; i++) {
            hasEdge = false;
            for (int j = 0; j < v; j++) {
                if (g[i][j] != NO_EDGE) {
                    hasEdge = true;
                    break;
                }
            }

            if (!hasEdge) {
                // None found, add one to a random other vertex
                newEdge = rand() % v;
                while (newEdge == i) {
                    newEdge = rand() % v;
                }

                g[i][newEdge] = randWeight();
            }
        }
    }

    void initializeGraph(Graph g) {
        for (int i = 0; i < g.length(); i++) {
            for (int j = 0; j < i; j++) {
                if (includeNext()) {
                    g.addEdge(i, j, randWeight());
                }
            }
        }

        // Make sure there's at least one edge on each vertex
        bool hasEdge = false;
        int newEdge = 0;
        for (int i = 0; i < g.length(); i++) {
            hasEdge = false;
            for (int j = 0; j < g.length(); j++) {
                if (g.weight(i, j) != NO_EDGE) {
                    hasEdge = true;
                    break;
                }
            }

            if (!hasEdge) {
                // None found, add one to a random other vertex
                newEdge = rand() % g.length();
                while (newEdge == i) {
                    newEdge = rand() % g.length();
                }

                g.addEdge(i, newEdge, randWeight());
            }
        }
    }
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
    g.pEdgeAdd = ceil(0.05 * v);
    g.pEdgeIn = v;

    return g;
}

#endif //MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
