#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MATRIX_SIZE 3

// Estrutura para passar os argumentos para a thread
typedef struct {
    float (*A)[MATRIX_SIZE];
    float (*B)[MATRIX_SIZE];
    float (*result)[MATRIX_SIZE];
    int start_row;
    int end_row;
} ThreadArgs;

// Função para multiplicação matricial
void* multiplyMatrix(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    for (int i = thread_args->start_row; i < thread_args->end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_args->result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                thread_args->result[i][j] += thread_args->A[i][k] * thread_args->B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

// Função para multiplicação posicional
void multiplyPositional(float A[MATRIX_SIZE][MATRIX_SIZE], float B[MATRIX_SIZE][MATRIX_SIZE], float result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = A[i][j] * B[i][j];
        }
    }
}

int main() {
    float matrixA[MATRIX_SIZE][MATRIX_SIZE] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
    float matrixB[MATRIX_SIZE][MATRIX_SIZE] = {{9.0, 8.0, 7.0}, {6.0, 5.0, 4.0}, {3.0, 2.0, 1.0}};
    float result[MATRIX_SIZE][MATRIX_SIZE];

    // Matriz de argumentos para a thread
    ThreadArgs thread_args = {matrixA, matrixB, result, 0, MATRIX_SIZE};

    // Criação da thread
    pthread_t thread;
    clock_t start, end;
    double elapsed_time;

    // Medir o tempo para multiplicação matricial com multithread
    start = clock();
    pthread_create(&thread, NULL, multiplyMatrix, (void*)&thread_args);
    pthread_join(thread, NULL);
    end = clock();

    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Multiplicação Matricial Multithread:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%.2f ", result[i][j]);
        }
        printf("\n");
    }

    printf("Tempo de execução da multiplicação matricial multithread: %.6f segundos\n", elapsed_time);

    // Medir o tempo para multiplicação posicional
    start = clock();
    multiplyPositional(matrixA, matrixB, result);
    end = clock();
    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nMultiplicação Posicional:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%.2f ", result[i][j]);
        }
        printf("\n");
    }

    printf("Tempo de execução da multiplicação posicional: %.8f segundos\n", elapsed_time);

    return 0;
}
