#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NTHREADS 2

int primes[10] = { 2, 3, 5, 7, 11, 12+1, 17, 19, 23, 29};

void *routine(void *arg)
{
    int index = *(int*)arg;
    int sum = 0;

    for (int j = 0; j < 5; j++)
    {
        sum += primes[index + j];
    }

    printf("Local sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    int i;
    

    for (i = 0; i < NTHREADS; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if(pthread_create(&threads[i], NULL, &routine, a)) return -1;
    }

    int globalSum = 0;
    for (i = 0; i < NTHREADS; i++)
    {
        int *r;
        if(pthread_join(threads[i], (void**) &r)) return -1;
        
        globalSum += *r;
        free(r);
    }

    printf("Global sum: %d\n", globalSum);

    return 0;
}