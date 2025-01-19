#include "thread.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define MATRIX_XSIZE 100
#define MATRIX_YSIZE 100
#define BLOCK_SIZE 3

int prime_count = 0;
clock_t start, finish;
double time_spent;

int **create_matrix(){
    // Set random seed for matrix
    srand(4);

    if (MATRIX_XSIZE < 1 || MATRIX_YSIZE < 1){ 
        printf("** Erro: Parametro Invalido **\n");
        return(NULL);
    }

    // Geração de uma matriz de números naturais aleatórios (intervalo 0 a 31999)
    int **matrix = (int **)malloc(MATRIX_XSIZE * sizeof(int *));
    
    if (!matrix) {
        printf("** Erro: Memoria Insuficiente **");
        return(NULL);
    }

    for (int i = 0; i < MATRIX_XSIZE; i++){

        matrix[i] = (int *)malloc(MATRIX_YSIZE * sizeof(int));

        if (!matrix[i]) {
            printf("** Erro: Memoria Insuficiente **");
            return(NULL);
        }

        for (int j = 0; j < MATRIX_YSIZE; j++){
            matrix[i][j] = rand() % 32000;
        } 
    }
    return matrix;
}

void print_matrix(int** matrix){
    for (int i = 0; i < MATRIX_XSIZE; i++){
        for (int j = 0; j < MATRIX_YSIZE; j++){
            printf("%.5d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(int** matrix){ 
    // Libera a memória alocada para a matriz
    for (int i = 0; i < MATRIX_XSIZE; i++){
        free(matrix[i]);
    }
    free(matrix);
}

int main(){

    // Contador de primos serial
    int **matrix = create_matrix();
    //print_matrix(matrix);

    /*  SERIAL PRIME COUNTING TEST - WORKING ON 100X100 MATRIX*/
    start = clock();

    prime_count = serial_count(matrix, MATRIX_XSIZE, MATRIX_YSIZE, prime_count);

    finish = clock();
    time_spent = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("\n** Quantidade de Primos: %d **\n", prime_count);
    printf("** Tempo de Execucao: %.6f segundos **\n", time_spent);

    /*  PARALLEL PRIME COUNTING TEST - WORK IN PROGRESS */
    /*
    start = clock();

    prime_count = parallel_count(matrix, MATRIX_XSIZE, MATRIX_YSIZE, BLOCK_SIZE, prime_count);

    finish = clock();
    time_spent = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("\n** Quantidade de Primos: %d **\n", prime_count);
    printf("** Tempo de Execucao: %.6f segundos **\n", time_spent);
    */

    // freeing the matrix after the tests
    free_matrix(matrix);

    return 0;
}
