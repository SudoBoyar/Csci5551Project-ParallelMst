//
// Created by Alex on 11/3/2017.
//

#ifndef MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
#define MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H

#include <stdlib.h>
#include <math.h>

/********************
 * Default settings *
 ********************/
struct GraphGenParams {
    // pEdgeAdd/pEdgeIn = probably of adding each edge
    int pEdgeAdd = 1;
    int pEdgeIn = 1000;

    int maxWeight = 100;

    bool includeNext() {
        return rand() % pEdgeIn < pEdge;
    }

    /**
     * Initialize the graph
     * @param g
     * @param v
     */
    void initializeGraph(int *g, int v) {
        g = new int *[v];
        g[0] = new int[v * v];
        for (int i = 1; i < v; i++) {
            g[i] = g[i - 1] + v;
        }

        for (int i = 0; i < v; i++) {
            for (int j = 0; j < i; j++) {
                if (includeNext()) {
                    g[i][j] = g[j][i] = rand() % maxWeight + 1;
                }
            }
        }

        // Make sure there's at least one edge on each vertex
        bool hasEdge = false;
        int newEdge = 0;
        for ( int i = 0; i < v; i++) {
            hasEdge = false;
            for (int j = 0; j < v; j++) {
                if (g[i][j] > 0) {
                    hasEdge = true;
                    continue;
                }
            }

            if (!hasEdge) {
                // None found, add one to a random other vertex
                newEdge = rand() % v;
                while (newEdge == i) {
                    newEdge = rand() % v;
                }
            }
        }
    }
};

GraphGenParams graphGenDense(int v) {
    GraphGenParams g = GraphGenParams();

    // Connect to ~95%
    g.pEdgeAdd = v - ceil(0.95 * v);
    g.pEdgeIn = v;

    return g;
}

GraphGenParams graphGenSparse(int v) {
    GraphGenParams g = GraphGenParams();

    // Connect to ~5%
    g.pEdgeAdd = ceil(0.05 * v);
    g.pEdgeIn = v;
}

#endif //MPIGRAPHALGORITHMS_INITIALIZEGRAPH_H
