#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct
{
    int id;
    int *vetor;
    int n;
} ThreadArgs;

void *tarefa_thread(void*arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    
    int id = args->id;
    int n = args->n;
    int *vetor = args->vetor;

    int inicio = id * n;
    int fim = inicio + n;

    printf("Thread %d: processando elementos do indice %d ao %d\n", id, inicio, fim-1);

    for (int i = inicio; i < fim; i++)
        vetor[i]++;

    pthread_exit(NULL);
}

void inicializa_vetor(int *vetor, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i] = i;
    }
}

void verifica_resultado(int *vetor, int tamanho)
{
    printf("Verificando o resultado..\n");

    for (int i = 0; i < tamanho; i++)
    {
        if (vetor[i] != i + 1)
        {
            printf("ERRO O valor no indice %d esta incorreto! (Valor: %d, Esperado: %d)\n", i, vetor[i], i+1);
            return;
        }
    }

    printf("SUCESSO! Todos os elementos foram incrementados corretamente!");
}

int main(int argc, char *argv[]){
    if (argc < 2)
    {
        printf("Uso: %s <valor_de_N>\n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]);
    if(N <= 0)
    {
        printf("O valor de N deve ser um inteiro positivo. \n");
        return -1;
    }

    int tamanho_vetor = NUM_THREADS * N;

    int *vetor = (int *)malloc(tamanho_vetor*sizeof(int));
    if(vetor == NULL)
    {
        printf("Erro ao alocar memoria para o vetor. \n");
        return -1;
    }

    inicializa_vetor(vetor, tamanho_vetor);

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    printf("\nCriando %d threads.....\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        args[i].id = i;
        args[i].n = N;
        args[i].vetor = vetor;
        
        if (pthread_create(&threads[i], NULL, tarefa_thread, (void * )&args[i]))
        {
            printf("ERRO AO CRIAR THREAD %d", i);
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("pthread_join executado com sucesso");

    verifica_resultado(vetor, tamanho_vetor);

    free(vetor);

    return 0;
}