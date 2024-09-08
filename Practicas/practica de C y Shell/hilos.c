#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 3

typedef struct {
    double (*matrix)[SIZE+1];
    double *result;
    int row;
} ThreadData;

void *gaussianEliminationThread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int i = data->row;
    double (*matrix)[SIZE+1] = data->matrix;

    for (int j = i + 1; j < SIZE; j++) {
        double factor = matrix[j][i] / matrix[i][i];
        for (int k = i; k <= SIZE; k++) {
            matrix[j][k] -= factor * matrix[i][k];
        }
    }

    pthread_exit(0);
}

void backwardSubstitution(double matrix[SIZE][SIZE+1], double result[SIZE]) {
    for (int i = SIZE - 1; i >= 0; i--) {
        result[i] = matrix[i][SIZE];
        for (int j = i + 1; j < SIZE; j++) {
            result[i] -= matrix[i][j] * result[j];
        }
        result[i] /= matrix[i][i];
    }
}

int main() {
    double matrix[SIZE][SIZE+1] = {
        {2, -1, 1, 8},
        {-3, -1, 2, -11},
        {-2, 1, 2, -3}
    };
    double result[SIZE];
    pthread_t threads[SIZE];
    ThreadData threadData[SIZE];

    printf("Sistema de ecuaciones:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j <= SIZE; j++) {
            printf("%8.3f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    for (int i = 0; i < SIZE; i++) {
        threadData[i].matrix = matrix;
        threadData[i].row = i;
        pthread_create(&threads[i], NULL, gaussianEliminationThread, &threadData[i]);
    }

    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    backwardSubstitution(matrix, result);

    printf("Soluciones:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("x%d = %.3f\n", i + 1, result[i]);
    }

    return 0;
}
