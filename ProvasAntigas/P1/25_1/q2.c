#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 4
#define M 100

typedef struct {
    int id; //id de cada thread
    int qtde; //quantidade de elementos por thread
} t_args;

float *vetor;

void *soma(void *arg){
    t_args *args = (t_args*) arg;
    long local = 0;
    int ini = args->qtde * args->id;
    int fim = ini + args->qtde;

    for(int i = ini; i < fim; i++)
        local += vetor[i];

    return (void*)local;
}

int main(){
    float soma_total = 0;
    pthread_t tid[N];         // fixed size array of threads
    t_args args[N];          // pre-allocate all args structures
    int elementos_por_thread = M/N;

    // Allocate and initialize vector
    vetor = malloc(sizeof(float) * M);
    if(!vetor) {
        printf("Error allocating vector\n");
        return -1;
    }

    for(int i = 0; i < M; i++){
        vetor[i] = 1.0;
    }

    // Create threads
    for(int i = 0; i < N; i++){
        args[i].id = i;
        args[i].qtde = elementos_por_thread;

        if(pthread_create(&tid[i], NULL, soma, (void*)&args[i])){
            printf("Error creating thread %d\n", i);
            free(vetor);
            return -1;
        }
    }

    // Join threads and sum results
    for(int i = 0; i < N; i++){
        void *partial_sum;

        if(pthread_join(tid[i], &partial_sum)){
            printf("Error joining thread %d\n", i);
            continue;
        }

        soma_total += (long)partial_sum;
    }

    printf("Soma total: %f\n", soma_total);
    free(vetor);
    return 0;
}