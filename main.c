#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

void *sample()
{
    printf("Thread sample. \n");
    
}
int main()
{
    pthread_t tid_1, tid_2;
    pthread_create(&tid_1, NULL, &sample, NULL);
    pthread_create(&tid_2, NULL, &sample, NULL);
    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

    return 0;
}


