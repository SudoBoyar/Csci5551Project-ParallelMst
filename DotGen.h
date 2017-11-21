//
// Created by Alex on 11/21/2017.
//

#ifndef MPIGRAPHALGORITHMS_DOTGEN_H
#define MPIGRAPHALGORITHMS_DOTGEN_H

#import <string>

void matrixOutput(int **g, int v, string filename) {
    FILE * outputFile = fopen(filename, "w");
    fprintf("strict graph {\n");
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < i; j++) {
            if (g[i][j] > 0) {
                fprintf("  %d -- %d [ID=\"%d\"]\n", i, j, g[i][j]);
            }
        }
    }

    fprintf("}\n");
}

#endif //MPIGRAPHALGORITHMS_DOTGEN_H
