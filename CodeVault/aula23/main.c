#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 2

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void *producer(void *args)
{
    while (1)
    {
        // produce
        int x = rand() % 100;
        sleep(1);

        // add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);

        buffer[count] = x;
        count++;
        printf("Produced (%d) and added to the buffer\n", x);
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
    
}

void *consumer(void *args)
{
    while (1)
    {
        int y;

        // remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);

        y = buffer[count - 1];
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // consume
        printf("Got (%d) from the buffer\n", y);
        sleep(1);
    }
    
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    int i;

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) 
            {
                perror("Failed to create producer thread");
                exit(EXIT_FAILURE);
            }
            else
            {
                if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) 
                {
                    perror("Failed to create consumer thread");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}