#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 2

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *routine(void *args)
{
    int index = *(int*)args;
    int sum = 0;
    sleep(1);

    for (int j = 0 ; j < 5; j++)
        sum += primes[index + j];
    printf("Local sum: %d\n", sum);
    *((int*)args) = sum;
    return args;
}

int main(int argc, char *argv[])
{
    pthread_t threads[THREAD_NUM];
    int *result;
    int total = 0;

    for (int i = 0; i < THREAD_NUM; i++) {
        int *a = (int*)malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&threads[i], NULL, routine, a) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], (void**)&result) != 0) {
            perror("Failed to join thread");
            return 1;
        }
        total += *result;
        free(result);
    }

    printf("Total sum: %d\n", total);
    return 0;
}