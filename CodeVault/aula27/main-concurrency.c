#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 16

int mails = 0;
pthread_mutex_t mutex;

void *routine()
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
        printf("Processing mail #%d\n", mails);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[])
{
    pthread_t threads[THREAD_NUM];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&threads[i], NULL, (void *)routine, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&mutex);

    printf("All mails processed: %d\n", mails);
    return 0;
}