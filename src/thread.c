#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int serial_count(int** matrix, int matrix_xsize, int matrix_ysize, int prime_count){
    for(int i = 0; i < matrix_xsize; i++){
        for(int j = 0; j < matrix_ysize; j++){
            if(isPrime(matrix[i][j])){
                prime_count++;
            }
        }
    }
    return prime_count;
}

int parallel_count(int** matrix, int matrix_xsize, int matrix_ysize, int block_size, int prime_count){

    return 0;
}

