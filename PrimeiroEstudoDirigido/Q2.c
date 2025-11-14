#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int *array;
    int start;
    int end;
    int local_min;
    int local_max;
} thread_arg;

void *find(void * arg)
{
    thread_arg *args = (thread_arg*)arg;

    args->local_min = args->array[args->start];
    args->local_max = args->array[args->end];

    for (int i = args->start + 1; i < args->end; i++)
    {
        if(args->array[i] < args->local_min)
        {
            args->local_min = args->array[i];
        }
        if(args->array[i] > args->local_max)
        {
            args->local_max = args->array[i];
        }
    }

    pthread_exit(NULL);
}

int main()
{
    const int N = 16;
    const int M = 4;

    int array[16] = {-1, 0, 10, -4, 5, 6, 7, 8, 9,83, -32, 12, 43, 23, 75, 65}; 

    pthread_t threads[M];
    thread_arg args[M];

    int chunck_size = N/M;

    for (int i = 0; i < M; i++)
    {
        args[i].array = array;
        args[i].start = i * chunck_size;
        args[i].end = (i+1)*chunck_size;
        pthread_create(&threads[i], NULL, find, &args[i]);
    }

    for(int i = 0; i < M; i++)
    {
        pthread_join(threads[i], NULL);
    }

    int global_min = args[0].local_min;
    int global_max = args[0].local_max;

    for (int i = 1; i < M; i++)
    {
        if (args[i].local_min < global_min)
            global_min = args[i].local_min;
        
        if (args[i].local_max > global_max)
            global_max = args[i].local_max;
    }

    printf("Min: %d, Max: %d\n", global_min, global_max);

    return 0;
}