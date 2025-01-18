#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int serial_count(int matrix_xsize, int matrix_ysize, int prime_count){
    for(int i = 0; i < matrix_xsize; i++){
        for(int j = 0; j < matrix_ysize; j++){
            if(is_prime(matrix[i][j])){
                prime_count++;
            }
        }
    }
    return prime_count;
}

int parallel_count(int block_size, int prime_count){

}

