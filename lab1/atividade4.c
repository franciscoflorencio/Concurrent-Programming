#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *vetor;
long N;

typedef struct {
    int id;
} thread_args;

void inicializa_vetor(int tamanho)
{
    printf("Inicializando o vetor com %d elementos (todos com valor 0).\n", tamanho);
    for (int i = 0; i < tamanho; i++){
        vetor[i] = 0;
    }
}

void verifica_vetor(int tamanho)
{
    printf("Verificando o resultado final...\n");
    for (int i = 0; i < tamanho; i++){
        if (vetor[i] !=)
            printf("Erro! O valor na posição %d é %d, mas deveria ser 1.\n", i, vetor[i]);
            return;
    }
}