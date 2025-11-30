#include <semaphore.h>

#define N 10

int Buffer[N];
sem_t slotCheio, slotVazio;
sem_t mutexProd, mutexCons;

// inicializador
void init () {
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, N);
}

// produtor
void Insere (int item) {
    static int in = 0;

    sem_wait(&slotVazio);
    sem_wait(&mutexProd);

    Buffer[in] = item;
    in = (in + 1) % N;

    sem_post(&mutexProd);
    sem_post(&slotCheio);
}

// consumer
int Retira (void) {
    int item;
    static int out = 0;

    sem_wait(&slotCheio);
    sem_wait(&mutexCons);

    item = Buffer[out];
    out = (out + 1) % N;

    sem_post(&mutexCons);
    sem_post(&slotVazio);

    return item;
}