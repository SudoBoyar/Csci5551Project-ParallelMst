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
#define SEED 1234


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

    weight_t randWeight() {
        return rand() % (2 * maxWeight) - maxWeight;
    }

    void fillNoEdge(weight_t **&g, int v) {
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
    void initializeGraph(weight_t **&g, int v) {
        g = new weight_t *[v];
        g[0] = new weight_t[(long) v * (long) v];
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
    }
};

void seed() {
    srand(SEED);
}

GraphGenParams noEdges(int v, bool useSeed) {
    GraphGenParams g = GraphGenParams();
    g.pEdgeAdd = 0;

    if (useSeed) seed();

    return g;
}

GraphGenParams graphGenDense(int v, bool useSeed) {
    GraphGenParams g = GraphGenParams();

    // Connect to ~95%
    g.pEdgeAdd = ceil(0.95 * v);
    g.pEdgeIn = v;

    if (useSeed) seed();

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
