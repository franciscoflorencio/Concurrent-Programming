#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 12+1, 17, 19, 23, 29};

void *routine(void *arg)
{
    int index = *(int*)arg;
    printf("%d ", primes[index]);
    free(arg);
}

int main(int argc, char *argv[])
{
    pthread_t threads[10];
    int i;
    

    for (i = 0; i < 10; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&threads[i], NULL, &routine, a)) return -1;
    }

    for (i = 0; i < 10; i++)
    {
        if(pthread_join(threads[i], NULL)) return -1;
    }

    return 0;
}