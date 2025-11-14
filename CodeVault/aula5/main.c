#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

//RACE CONDITION
void *routine()
{
    for (int i = 0; i < 100000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t threads[12];
    int i;
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 12; i++)
    {
        if(pthread_create(threads + i, NULL, &routine, NULL)) return -1;
        printf("Thread %d has started\n", i);
    }

    for (i = 0; i < 12; i++)
    {
        if(pthread_join(threads[i], NULL)) return -2;
        printf("Thread %d has finished\n", i);

    }

    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;

}