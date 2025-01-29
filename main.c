#pragma once
#pragma comment(lib,"pthreadVC2.lib")

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>

//#include <unistd.h>

#define SEED 4
#define NUM_THREADS 8
#define MATRIX_XSIZE 10000
#define MATRIX_YSIZE 10000
#define BLOCK_XSIZE 100
#define BLOCK_YSIZE 100

int** matrix;
int prime_count;
double time_spent;
clock_t start, finish;
int blocks_insider_matrix;

// Mutexes de controle de RC's
pthread_mutex_t mutex_count;
pthread_mutex_t mutex_queue;

/* *************************** */
/* ***** QUEUE FUNCTIONS ***** */
/* *************************** */
typedef struct block_coord BlockCoord;
typedef struct block_queue_node BlockQueueNode;
typedef struct block_queue BlockQueue;

struct block_coord {
    int block_xposition;
    int block_yposition;
};

// Define a structure for a node in the linked list.
struct block_queue_node
{
    BlockCoord* info;
    BlockQueueNode* next; // A pointer to the next node in the linked list.
};

// Define the structure for the block_queue.
struct block_queue
{
    BlockQueueNode* front; // Pointer to the front of the block_queue.
    BlockQueueNode* rear;  // Pointer to the rear of the block_queue.
};

BlockQueue* block_queue;

// Function to create a new empty block_queue.
BlockQueue* q_create()
{
    BlockQueue* q = (BlockQueue*)malloc(sizeof(BlockQueue));// Allocate memory for the block_queue structure.
    if (q == NULL) return NULL;                 // If the memory allocation fails, return NULL (indicating failure
    q->front = q->rear = NULL;                 // Initialize the front and rear pointers to NULL, indicating an empty block_queue.
    return q;
}

// Function to check whether the block_queue is empty.
int q_is_empty(BlockQueue* q)
{
    return q->front == NULL;
}

// Function to enqueue a float value into the block_queue.
void q_enqueue(BlockQueue* q, BlockCoord* v)
{
    BlockQueueNode* node = (BlockQueueNode*)malloc(sizeof(BlockQueueNode)); // Allocate memory for a new node.
    if (node == NULL) {
        printf("** Error: Not enough memory **\n");
        return;
    }
    node->info = v;
    node->next = NULL;

    if (q_is_empty(q))
        q->front = node;
    else
        q->rear->next = node;

    q->rear = node;
}

// Function to dequeue and return a value from the block_queue.
BlockCoord* q_dequeue(BlockQueue* q)
{
    assert(!q_is_empty(q));

    BlockCoord* v = q->front->info;
    BlockQueueNode* p = q->front; // Store for removal

    if (q->front != q->rear)
        q->front = q->front->next;
    else
        q->front = q->rear = NULL;

    free(p);
    return v;
}

// Function to free the memory used by the block_queue.
void q_free(BlockQueue* q)
{
    BlockQueueNode* p = q->front;
    while (p != NULL)
    {
        BlockQueueNode* t = p->next; // Store a reference to the next node.
        free(p);                // Free the memory allocated for the current node.
        p = t;                  // Move to the next node.
    }
    free(q); // Free the memory allocated for the block_queue structure itself.
}

void q_print(BlockQueue* q) {
    BlockQueueNode* current = q->front;
    while (current != NULL)
    {
        printf("(%d, %d) ", current->info->block_xposition, current->info->block_yposition);
        current = current->next;
    }
    printf("\n");
}

/* **** BLOCK COORDINATES **** */
BlockCoord* create_coord_struct(int x, int y) {
    BlockCoord* coord = (BlockCoord*)malloc(sizeof(BlockCoord));
    if (coord == NULL) {
        printf("** Error: Not enough memory **\n");
        return NULL;
    }
    coord->block_xposition = x;
    coord->block_yposition = y;
    blocks_insider_matrix += 1;
    return coord;
}

void print_block_queue(BlockQueue* block_queue) {
    BlockQueueNode* current = block_queue->front;

    while (current != NULL) {
        BlockCoord* coord = current->info;
        printf("Block at coordinate (%d, %d):\n", coord->block_xposition, coord->block_yposition);

        for (int i = coord->block_xposition; i < coord->block_xposition + BLOCK_XSIZE && i < MATRIX_XSIZE; i++) {
            for (int j = coord->block_yposition; j < coord->block_yposition + BLOCK_YSIZE && j < MATRIX_YSIZE; j++) {
                printf("%5d ", matrix[i][j]);
            }
            printf("\n");
        }

        printf("\n");
        current = current->next;
    }
}

/* **************************** */
/* ***** MATRIX FUNCTIONS ***** */
/* **************************** */

int isPrime(int number) {
    // if the number is less than or equal to 1, it is not prime.
    if (number <= 1) {
        return 0;
    }

    int square_root = (int)sqrt(number);
    // Check for divisors from 2 to the square root of the number.
    // If any divisor is found, the number is not prime.
    for (int i = 2; i <= square_root; i++) {
        if (number % i == 0) {
            return 0;
        }
    }

    // If no divisors are found, the number is prime.
    return 1;
}

int** allocate_matrix() {
    int** v; /* a pointer to array */
    int index;

    if (MATRIX_XSIZE < 1) {
        printf("** Error: Invalid MATRIX_XSIZE **\n");
        return (NULL);
    }

    if (MATRIX_YSIZE < 1) {
        printf("** Error: Invalid MATRIX_YSIZE **\n");
        return (NULL);
    }



    /* aloca as linhas da matriz */
    v = calloc(MATRIX_XSIZE, sizeof(int*)); /* An vector of m pointers to int */

    if (v == NULL) {
        printf("** Error: Not enough memory **\n");
        return (NULL);
    }

    /* aloca as colunas da matriz */
    for (index = 0; index < MATRIX_XSIZE; index++) {
        v[index] = calloc(MATRIX_YSIZE, sizeof(int)); /* m vectors of n int */

        if (v[index] == NULL) {
            printf("** Error: Not enough memory **\n");
            return (NULL);
        }
    }

    return (v); /* returns the pointer to the array */
}

void fill_matrix() {
    int i, j;
    srand(SEED);

    for (i = 0; i < MATRIX_XSIZE; i++) {
        for (j = 0; j < MATRIX_YSIZE; j++) {
            matrix[i][j] = rand() % 32000;
        }
    }
}

int** free_matrix() {
    int index;

    if (matrix == NULL) return (NULL);

    if (MATRIX_XSIZE < 1) {
        printf("** Error: MATRIX_XSIZE must be >= 1 **\n");
        return NULL;
    }
    if (MATRIX_YSIZE < 1) {
        printf("** Error: MATRIX_YSIZE must be >= 1 **\n");
        return NULL;
    }


    for (index = 0; index < MATRIX_XSIZE; index++) free(matrix[index]); /* free the matrix lines */

    free(matrix); /* free the array (vector of pointers) */

    return (NULL); /* returns a NULL pointer */
}

void print_matrix() {
    for (int i = 0; i < MATRIX_XSIZE; i++) {
        for (int j = 0; j < MATRIX_YSIZE; j++) {
            printf("%.5d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* **************************** */
/* ***** THREAD FUNCTIONS ***** */
/* **************************** */

void* thread_readblock(void* arg) {
    if (block_queue == NULL) {
        printf("**Error: block_queue is NULL **\n");
        pthread_exit(NULL);
    }

    while (1) {
        pthread_mutex_lock(&mutex_queue);
        if (q_is_empty(block_queue)) {
            pthread_mutex_unlock(&mutex_queue);
            break;  // Exit if queue is empty
        }
        BlockCoord* coord = q_dequeue(block_queue);
        pthread_mutex_unlock(&mutex_queue);

        if (coord == NULL) {
            break; // Extra safeguard
        }

        // Process the block
        for (int i = coord->block_xposition; i < coord->block_xposition + BLOCK_XSIZE && i < MATRIX_XSIZE; i++) {
            for (int j = coord->block_yposition; j < coord->block_yposition + BLOCK_YSIZE && j < MATRIX_YSIZE; j++) {
                if (isPrime(matrix[i][j])) {
                    pthread_mutex_lock(&mutex_count);
                    prime_count++;
                    pthread_mutex_unlock(&mutex_count);
                }
            }
        }
        //printf("\nprime_count at coord (%d, %d): %d\n", coord->block_xposition, coord->block_yposition, prime_count);
    }

    pthread_exit(NULL);
}



/* **************************** */
/* ***** COUNT FUNCTIONS ***** */
/* **************************** */

double serial_count() {
    start = clock();

    for (int i = 0; i < MATRIX_XSIZE; i++) {
        for (int j = 0; j < MATRIX_YSIZE; j++) {
            if (isPrime(matrix[i][j])) {
                prime_count++;
            }
        }
    }

    // Function reports
    finish = clock();
    time_spent = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("\n** Matrix Size: %dx%d            **\n", MATRIX_XSIZE, MATRIX_YSIZE);
    printf("** Prime numbers in matrix: %d   **\n", prime_count);
    prime_count = 0;
    return time_spent;
}

double parallel_count(BlockQueue* block_queue, pthread_t* threads) {

    start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        //printf("\n Created thread %d\n", i);
        int ret = pthread_create(&threads[i], NULL, thread_readblock, NULL);
        if (ret != 0) {
            printf("** Error creating thread %d **\n", i);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        int ret = pthread_join(threads[i], NULL);
        if (ret != 0) {
            printf("** Error: joining thread %d**\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //printf("Verificando BLOCK QUEUE \n");
    //print_block_queue(block_queue);

    // Function reports
    finish = clock();
    time_spent = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("\n** Matrix Size: %dx%d            **\n", MATRIX_XSIZE, MATRIX_YSIZE);
    printf("** Blocks inside the matrix: %d     **\n", blocks_insider_matrix);
    printf("** Threads used: %d                     **\n", NUM_THREADS);
    printf("** Prime numbers in matrix: %d   **\n", prime_count);

    prime_count = 0;
    return time_spent;
}

/* **********  MENU  ********** */
void menu(pthread_t* threads) {
    int option;
    double speedup, serial_time, parallel_time;

    do {
        printf("\n---------------- MENU -------------------\n");
        printf("1. Print Matrix\n");
        printf("2. Print Matrix's Blocks\n");
        printf("3. Count Prime Numbers (Serial)\n");
        printf("4. Count Prime Numbers (Parallel)\n");
        printf("5. Calculate Speedup (Runs both counters)\n");
        printf("0. Exit\n");
        printf("-----------------------------------------\n");

        if (scanf("%d", &option)) {
            system("cls || clear");
            printf("\n\n");
        }
        else {
            printf("** Error: Invalid option!!! **\n**  Please enter a valid option **\n");
            break;
        }

        switch (option)
        {
        case 1:
            printf("** Printing the full matrix...         **\n");
            printf("-----------------------------------------\n");
            print_matrix();

            break;

        case 2:
            printf("** Printing each block in matrix...    **\n");
            printf("-----------------------------------------\n");
            print_block_queue(block_queue);

            break;

        case 3:
            /* SERIAL PRIME COUNTING TEST */
            printf("** Running Serial Count...             **\n");
            printf("-----------------------------------------\n");
            serial_time = serial_count();
            printf("** Run time: %.6f seconds          **\n", serial_time);

            break;

        case 4:

            if (block_queue != NULL) {
                q_free(block_queue);  // Free previous queue if it exists
            }
            block_queue = q_create();  
            // Populate the queue with block start coordinates
            for (int i = 0; i < MATRIX_XSIZE; i += BLOCK_XSIZE) {
                for (int j = 0; j < MATRIX_YSIZE; j += BLOCK_YSIZE) {
                    q_enqueue(block_queue, create_coord_struct(i, j));
                }
            }

            /* PARALLEL PRIME COUNTING TEST */
            printf("** Creating %d threads...               **\n", NUM_THREADS);
            printf("-----------------------------------------\n");
            threads = malloc(NUM_THREADS * sizeof(pthread_t));

            if (threads == NULL) {
                perror("Error when allocating threads");
                break;
            }

            printf("\n** Running Parallel Count...           ** \n");
            printf("-----------------------------------------\n");
            parallel_time = parallel_count(block_queue, threads);
            printf("** Run time: %.6f seconds          **\n", parallel_time);

            // Free threads when they aren't needed anymore
            free(threads);

            break;

        case 5:

            if (block_queue != NULL) {
                q_free(block_queue);  // Free previous queue if it exists
            }
            block_queue = q_create();  
            // Populate the queue with block start coordinates
            for (int i = 0; i < MATRIX_XSIZE; i += BLOCK_XSIZE) {
                for (int j = 0; j < MATRIX_YSIZE; j += BLOCK_YSIZE) {
                    q_enqueue(block_queue, create_coord_struct(i, j));
                }
            }
            /* SERIAL PRIME COUNTING TEST */
            printf("** Running Serial Count...             **\n");
            printf("-----------------------------------------\n");
            serial_time = serial_count();
            printf("** Run time: %.6f seconds          **\n", serial_time);
            printf("\n-----------------------------------------\n");

            /* PARALLEL PRIME COUNTING TEST */
            printf("\n** Creating %d threads...               **\n", NUM_THREADS);
            printf("-----------------------------------------\n");
            threads = malloc(NUM_THREADS * sizeof(pthread_t));

            if (threads == NULL) {
                perror("Error when allocating threads");
                break;
            }
            

            printf("\n** Running Parallel Count...           ** \n");
            printf("-----------------------------------------\n");
            parallel_time = parallel_count(block_queue, threads);
            printf("** Run time: %.6f seconds          **\n", parallel_time);

            // Free threads when they aren't needed anymore
            free(threads);
            
            speedup = serial_time / parallel_time;
            printf("\n-----------------------------------------\n");
            printf("\n** Speedup: %.6f                   **\n", speedup);
            break;

        case 0:
            printf("** Menu exited with success            **\n");
            printf("-----------------------------------------\n");
            break;

        default:
            printf("** Error: Invalid option!!! **\n**  Please enter a valid option **\n");
        }
    } while (option);

}

/* **************************** */
/* **********  MAIN  ********** */
/* **************************** */
int main(int argc, char* argv[]) {

    pthread_t* threads = NULL;

    // Initialize mutexes
    pthread_mutex_init(&mutex_count, NULL);
    pthread_mutex_init(&mutex_queue, NULL);


    // Initialize matrix
    matrix = allocate_matrix();
    fill_matrix();
    //print_matrix();

    menu(&threads);

    // freeing the matrix after the tests
    free_matrix();

    // Destroy mutexes
    pthread_mutex_destroy(&mutex_count);
    pthread_mutex_destroy(&mutex_queue);

    return 0;

    system("pause");
}