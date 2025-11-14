#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 3
#define NTHREADS 3

pthread_mutex_t mutex;
pthread_cond_t cond;

void barreira(int numThreads){
    static int chegaram=0;
    pthread_mutex_lock(&mutex);
    chegaram++;
    if(chegaram < numThreads){
        pthread_cond_wait(&cond, &mutex);
    } else {
        chegaram = 0;
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
}

void *tarefa(void * args){
    long int id = (long int) args;
    for (int i = 0; i < MAX; i++)
    {
        printf("Thread %ld: etapa=%d\n", id, i);
        //barreira(NTHREADS);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t threads[NTHREADS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for(long int i = 0; i < NTHREADS; i++){
        pthread_create(&threads[i], NULL, tarefa, (void*)i);
    }

    for(long int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("FIM.\n");
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}