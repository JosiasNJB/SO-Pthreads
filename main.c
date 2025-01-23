#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>

#define MATRIX_XSIZE 9
#define MATRIX_YSIZE 9
#define BLOCK_XSIZE 3
#define BLOCK_YSIZE 3

int prime_count = 0;

clock_t start, finish;

double time_spent;

/* *************************** */
/* ***** QUEUE FUNCTIONS ***** */
/* *************************** */

typedef struct block_coord BlockCoord;
typedef struct block_queue_node BlockQueueNode;
typedef struct block_queue BlockQueue;

struct block_coord{
    int block_xposition;
    int block_yposition;
};

// Define a structure for a node in the linked list.
struct block_queue_node
{
   BlockCoord *info;
   BlockQueueNode *next; // A pointer to the next node in the linked list.
};

// Define the structure for the block_queue.
struct block_queue
{
   BlockQueueNode *front; // Pointer to the front of the block_queue.
   BlockQueueNode *rear;  // Pointer to the rear of the block_queue.
};

// Function to create a new empty block_queue.
BlockQueue *q_create()
{
   BlockQueue *q = (BlockQueue *)malloc(sizeof(BlockQueue)); // Allocate memory for the block_queue structure.
   q->front = q->rear = NULL;                 // Initialize the front and rear pointers to NULL, indicating an empty block_queue.
   return q;
}

// Function to check whether the block_queue is empty.
int q_is_empty(BlockQueue *q)
{
   return q->front == NULL;
}

// Function to enqueue a float value into the block_queue.
void q_enqueue(BlockQueue *q, BlockCoord *v)
{
   BlockQueueNode *node = (BlockQueueNode *)malloc(sizeof(BlockQueueNode)); // Allocate memory for a new node.
   node->info = v;
   node->next = NULL;

   if (q_is_empty(q))
      q->front = node;
   else
      q->rear->next = node;

   q->rear = node;
}

// Function to dequeue and return a float value from the block_queue.
BlockCoord *q_dequeue(BlockQueue *q)
{
   assert(!q_is_empty(q));

   BlockCoord *v = q->front->info;
   BlockQueueNode *p = q->front; // Store for removal

   if (q->front != q->rear)
      q->front = q->front->next;
   else
      q->front = q->rear = NULL;

   free(p);
   return v;
}

// Function to free the memory used by the block_queue.
void q_free(BlockQueue *q)
{
   BlockQueueNode *p = q->front;
   while (p != NULL)
   {
      BlockQueueNode *t = p->next; // Store a reference to the next node.
      free(p);                // Free the memory allocated for the current node.
      p = t;                  // Move to the next node.
   }
   free(q); // Free the memory allocated for the block_queue structure itself.
}

void q_print(BlockQueue *q){
    BlockQueueNode *current = q->front;
    while (current!= NULL)
    {
        printf("(%d, %d) ", current->info->block_xposition, current->info->block_yposition);
        current = current->next;
    }
    printf("\n");
}

void print_block_queue(BlockQueue *block_queue, int **matrix, int block_xsize, int block_ysize) {
    BlockQueueNode *current = block_queue->front;

    while (current != NULL) {
        BlockCoord *coord = current->info;
        printf("Bloco na coordenada (%d, %d):\n", coord->block_xposition, coord->block_yposition);

        for (int i = coord->block_xposition; i < coord->block_xposition + block_xsize && i < MATRIX_XSIZE; i++) {
            for (int j = coord->block_yposition; j < coord->block_yposition + block_ysize && j < MATRIX_YSIZE; j++) {
                printf("%5d ", matrix[i][j]);
            }
            printf("\n");
        }

        printf("\n"); 
        current = current->next;
    }
}

/* **** BLOCK COORDINATES **** */
BlockCoord *create_coord_struct(int x, int y){
   BlockCoord *coord = (BlockCoord *)malloc(sizeof(BlockCoord));
   coord-> block_xposition = x;
   coord-> block_yposition = y;

   return coord;
}





/* **************************** */
/* ***** MATRIX FUNCTIONS ***** */
/* **************************** */

int isPrime(int x){
    // if the number is less than or equal to 1, it is not prime.
    if (x <= 1) {
        return 0;
    }

    int square_root = sqrt(x);
    // Check for divisors from 2 to the square root of the number.
    // If any divisor is found, the number is not prime.
    for (int i = 2; i <= square_root; i++){
        if (x % i == 0){
            return 0;
        }
    }

    // If no divisors are found, the number is prime.
    return 1;
}

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


/* **************************** */
/* ***** THREAD FUNCTIONS ***** */
/* **************************** */



/* **************************** */
/* ***** COUNT FUNCTIONS ***** */
/* **************************** */

int serial_count(int **matrix, int matrix_xsize, int matrix_ysize, int prime_count){
    for(int i = 0; i < matrix_xsize; i++){
        for(int j = 0; j < matrix_ysize; j++){
            if(isPrime(matrix[i][j])){
                prime_count++;
            }
        }
    }
    return prime_count;
}

int parallel_count(int **matrix, int matrix_xsize, int matrix_ysize, int block_xsize, int block_ysize, int prime_count){
    (void)matrix;
    (void)matrix_xsize;
    (void)matrix_ysize;
    (void)block_xsize;
    (void)block_ysize;
    (void)prime_count;
    return 0;
}


/* **************************** */
/* **********  MAIN  ********** */
/* **************************** */

int main(){
    int i = 0, j = 0;
    int block_xnum = MATRIX_XSIZE / BLOCK_XSIZE;
    int block_ynum = MATRIX_YSIZE / BLOCK_YSIZE;

    // Contador de primos serial
    int **matrix = allocate_matrix();
    fill_matrix(matrix);
    print_matrix(matrix);

    // Starting the queue with the start coordinates of every block
    BlockQueue *block_queue = q_create();
    
    for (int i = 0; i < MATRIX_XSIZE; i += BLOCK_XSIZE) {
        for (int j = 0; j < MATRIX_YSIZE; j += BLOCK_YSIZE) {
            q_enqueue(block_queue, create_coord_struct(i, j));
        }
    }

    q_print(block_queue);

    print_block_queue(block_queue, matrix, BLOCK_XSIZE, BLOCK_YSIZE);

    /*  SERIAL PRIME COUNTING TEST - WORKING */
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