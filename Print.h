//
// Created by Alex Klein on 12/2/17.
//

#ifndef CSCI5551PROJECT_PARALLELMST_PRINT_H
#define CSCI5551PROJECT_PARALLELMST_PRINT_H

void Print2DMatrix(weight_t **a, int n) {
    for (int i = 0; i < n; i++) {
        cout << "Row " << (i + 1) << ":\t";
        for (int j = 0; j < n; j++) {
            printf("%d\t", a[i][j]);
        }
        cout << endl;
    }
}

#endif //CSCI5551PROJECT_PARALLELMST_PRINT_H
