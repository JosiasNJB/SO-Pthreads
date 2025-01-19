#ifndef THREAD_H
#define THREAD_H

// Counting the prime numbers in a matrix
int serial_count(int** matrix, int matrix_xsize, int matrix_ysize, int prime_count);

int parallel_count(int** matrix, int block_size, int prime_count);

#endif 