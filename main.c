#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define MATRIX_XSIZE 10
#define MATRIX_YSIZE 10
#define BLOCK_SIZE 3

int isPrime(int x){
    // if the number is less than or equal to 1, it is not prime.
    if (x <= 1) {
        return 0;
    }

    // Check for divisors from 2 to the square root of the number.
    // If any divisor is found, the number is not prime.
    for (int i = 2; i <= sqrt(x); i++){
        if (x % i == 0){
            return 0;
        }
    }

    // If no divisors are found, the number is prime.
    return 1;
}

int **allocate_matrix(){
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
    if (matrix == NULL) return NULL;

    if (MATRIX_XSIZE < 1 || MATRIX_YSIZE < 1){
        printf("** Erro: Parametro Invalido **\n");
        return(NULL);
    }
    
    // Libera a memória alocada para a matriz
    for (int i = 0; i < MATRIX_XSIZE; i++){
        free(matrix[i]);
    }
    free(matrix);
}

int main(){

    int **matrix = allocate_matrix();
    print_matrix(matrix);
    
    free_matrix(matrix);
    
    return 0;
}