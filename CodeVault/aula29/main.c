#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define THREAD_NUM 4

typedef struct Task {
    void (*taskFunction)(int, int);
    int arg1, arg2;
} Task;

Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void sumAndProduct(int a, int b)
{
    usleep(50000); // Simulate a time-consuming task
    int sum = a + b;
    int product = a * b;
    printf("Task: %d + %d = %d, %d * %d = %d\n",
         a, b, sum, a, b, product);
}

void sum(int a, int b)
{
    usleep(50000); // Simulate a time-consuming task
    int result = a + b;
    printf("Soma: %d + %d = %d\n", a, b, result);
}

void product(int a, int b)
{
    usleep(50000); // Simulate a time-consuming task
    int result = a * b;
    printf("Produto: %d * %d = %d\n", a, b, result);
}

void executeTask(Task* task)
{
    task->taskFunction(task->arg1, task->arg2);
}
 
void submitTask(Task task)
{
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void *startThread(void *args)
{
    while (1)
    {
        Task task;

        pthread_mutex_lock(&mutexQueue);

        while (taskCount == 0)
        {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue[0];
        int i;
        for (i = 0; i < taskCount - 1; i++) {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        
        executeTask(&task);
    }
}

int main(int argc, char *argv[])
{
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    int i;

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, startThread, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (i = 0; i < 10; i++)
    {
        Task t = 
        {
            .taskFunction = (i % 2 == 0) ? sum : product,
            .arg1 = rand() % 100,
            .arg2 = rand() % 100
        };
        submitTask(t);
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    return 0;
}