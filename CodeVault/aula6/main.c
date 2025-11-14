#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void *roll_dice()
{
    int value = (rand() % 6) + 1;
    int *result = malloc(sizeof(int));
    *result = value;
    //printf("%d\n", value);
    printf("Thread result: %p\n", result);
    return (void*) result;
}

int main(int argc, char *argv[])
{
    int *res;
    srand(time(NULL));
    pthread_t thread;

    if(pthread_create(&thread, NULL, &roll_dice, NULL)) return -1;

    if(pthread_join(thread, (void**) &res)) return -2;
    
    printf("Main res: %p\n", res);
    printf("Result: %d\n", *res);

    free(res);
    return 0;
}