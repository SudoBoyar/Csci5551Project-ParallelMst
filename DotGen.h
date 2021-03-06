//
// Created by Alex on 11/21/2017.
//

#ifndef MPIGRAPHALGORITHMS_DOTGEN_H
#define MPIGRAPHALGORITHMS_DOTGEN_H

#include <stdio.h>
#include <string>

using namespace std;

void matrixOutput(int **g, int v, const char *filename) {
    FILE *outputFile = fopen(filename, "w");
    fprintf(outputFile, "strict graph {\n");

    // Include individual vertices in case of vertex with no edges
    for (int i = 0; i < v; i++) {
        fprintf(outputFile, "  %d\n", i);
    }

    for (int i = 0; i < v; i++) {
        for (int j = i+1; j < v; j++) {
            if (g[i][j] != NO_EDGE) {
                fprintf(outputFile, "  %d -- %d [label=%d]\n", i, j, g[i][j]);
            }
        }
    }

    fprintf(outputFile, "}\n");

    fclose(outputFile);
}

void matrixMstOutput(int **g, int **mst, int v, const char *filename) {
    FILE *outputFile = fopen(filename, "w");
    fprintf(outputFile, "strict graph {\n");
    for (int i = 0; i < v; i++) {
        for (int j = i+1; j < v; j++) {
            if (mst[i][j] != NO_EDGE) {
                fprintf(outputFile, "  %d -- %d [label=%d,color=red,penwidth=3.0]\n", i, j, g[i][j]);
            } else if (g[i][j] != NO_EDGE) {
                fprintf(outputFile, "  %d -- %d [label=%d]\n", i, j, g[i][j]);
            }
        }
    }

    fprintf(outputFile, "}\n");

    fclose(outputFile);
}

#endif //MPIGRAPHALGORITHMS_DOTGEN_H
