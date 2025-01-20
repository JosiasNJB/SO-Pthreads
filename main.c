#include "thread.h"
#include "queue.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define MATRIX_XSIZE 10
#define MATRIX_YSIZE 10
#define BLOCK_XSIZE 2
#define BLOCK_YSIZE 2

int prime_count = 0;
clock_t start, finish;
double time_spent;

int **allocate_matrix(){
    int **matrix; /* ponteiro para a matriz */
    int index;

    if (MATRIX_XSIZE < 1 || MATRIX_YSIZE < 1) { /* verifica parametros recebidos */
        printf ("** Error: Invalid parameter **\n");
        return (NULL);
    }

    /* aloca as linhas da matriz */
    matrix = calloc(MATRIX_XSIZE, sizeof(int*)); /* Um vetor de m ponteiros para int */

    if (matrix == NULL) {
        printf("** Error: Not enough memory **\n");
        return (NULL);
    }

    /* aloca as colunas da matriz */
    for ( index = 0; index < MATRIX_XSIZE; index++ ) {
        matrix[index] = calloc(MATRIX_YSIZE, sizeof(int)); /* m vetores de n int */

        if (matrix[index] == NULL) {
            printf("** Error: Not enough memory **\n");
            return (NULL);
        }
 }

 return (matrix); /* retorna o ponteiro para a matriz */
}

void fill_matrix(int **matrix){
    int i, j;
    srand(4);

    for (i = 0; i < MATRIX_XSIZE; i++){
        for (j = 0; j < MATRIX_YSIZE; j++){
            matrix[i][j] = rand() % 32000;
        }
    }
}

int **free_matrix(int **matrix){
    int index; 

    if (matrix == NULL) return (NULL);

    if (MATRIX_XSIZE < 1 || MATRIX_YSIZE < 1) { /* verifica parâmetros recebidos */
        printf ("** Error: Invalid parameter **\n");
        return (matrix);
    }

    for (index = 0; index < MATRIX_XSIZE; index++) free (matrix[index]); /* libera as linhas da matriz */

    free (matrix); /* libera a matriz (vetor de ponteiros) */
    
    return (NULL); /* retorna um ponteiro nulo */
}

void print_matrix(int **matrix){
    for (int i = 0; i < MATRIX_XSIZE; i++){
        for (int j = 0; j < MATRIX_YSIZE; j++){
            printf("%.5d ", matrix[i][j]);
        }
        printf("\n");
    }
}
;

int main(){
    int i = 0, j = 0;
    int block_xnum = MATRIX_XSIZE / BLOCK_XSIZE;
    int block_ynum = MATRIX_YSIZE / BLOCK_YSIZE;

    // Contador de primos serial
    int **matrix = allocate_matrix();
    fill_matrix(matrix);
    //print_matrix(matrix);

    // Starting the queue with the start coordinates of every block
    BlockQueue *block_queue = q_create();
    
    // 
    while(i <= block_xnum && j <= block_ynum){
        //
        if(i < block_xnum ){
            q_enqueue(block_queue, create_coord_struct(i, j));
            q_print(block_queue);
            i++;
        }
        // 
        if(i == block_xnum ){
            q_enqueue(block_queue, create_coord_struct(i, j));
            q_print(block_queue);

            i=0;
            j++;
        }
    }

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
