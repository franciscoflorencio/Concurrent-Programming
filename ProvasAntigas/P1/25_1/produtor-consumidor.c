#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAMANHO_BUFFER 10
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 3
#define ITENS_POR_PRODUTOR 15

int buffer[TAMANHO_BUFFER];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_nao_cheio;
pthread_cond_t cond_nao_vazio;

void *produtor(void *arg){
    int id = *(int*)arg;
    for (int i = 0; i < ITENS_POR_PRODUTOR; i++)
    {
        int item = (id*1000)+i;

        pthread_mutex_lock(&mutex);

        while(count == TAMANHO_BUFFER){
            printf("Produtor %d: Buffer CHEIO. Esperando...\n", id);
            pthread_cond_wait(&cond_nao_cheio, &mutex);
        }

        buffer[in] = item;
        in = (in + 1) % TAMANHO_BUFFER;
        count++;
        printf("Produtor %d produziu -> %d (count=%d)\n", id, item, count);

        pthread_cond_signal(&cond_nao_vazio);
        pthread_unlock(&mutex);
        sleep(rand()%2);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg){
    int id = *(int*)arg;
    int itens_a_consumir = (NUM_PRODUTORES * ITENS_POR_PRODUTOR) / NUM_CONSUMIDORES;

    for (int i = 0; i < itens_a_consumir; i++)
    {
        pthread_mutex_lock(&mutex);

        while(count == 0){
            printf("Consumidor %d: Buffer VAZIO. Esperando...\n", id);
            pthread_cond_wait(&cond_nao_vazio, &mutex);
        }

        int item
    }
    
}