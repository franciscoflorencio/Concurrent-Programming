#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 4

int fuel = 50;

sem_t semFuel;
pthread_mutex_t mutexFuel;

void *routine1(void *args)
{
    sem_wait(&semFuel);
    fuel += 50;
    printf("Current value is %d\n", fuel);
}

void *routine2(void *args)
{
    sem_post(&semFuel);
    usleep(50000);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    fuel = 50;
    pthread_mutex_init(&mutexFuel, NULL);
    sem_init(&semFuel, 0, 1);
    int i;

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&th[i], NULL, &routine1, NULL) != 0)
            {
                perror("Failed to create thread");
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &routine2, NULL) != 0)
            {
                perror("Failed to create thread");
            }
        }
    }

    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }

    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&semFuel);
    return 0;
}