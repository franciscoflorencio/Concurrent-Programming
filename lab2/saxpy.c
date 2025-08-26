#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000

float vetorA[N], vetorB[N], vetorC[N];

typedef struct 
{
    int id;
    int fatia;
    int dim;
    float k;
} t_args;

void *somaVetores(void *arg);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return -1;
    }

    int dim = atoi(argv[1]);
    int nthreads = atoi(argv[2]);
    pthread_t threads[nthreads];

    if (dim <= 0 || nthreads <= 0)
    {
        fprintf(stderr, "Dimensao e numero de threads devem ser positivos.\n");
        return -1;
    }

    t_args args[nthreads];

    if (dim > N)
    {
        fprintf(stderr, "Dimensao do vetor nao pode ser maior que %d\n", N);
        return -1;
    }

    // Inicializa os vetores A e B
    for (int i = 0; i < dim; i++)
    {
        vetorA[i] = (float)i;
        vetorB[i] = (float)(i + 1);
    }

    // Cria as threads
    for (int i = 0; i < nthreads; i++)
    {
        args[i].id = i;
        args[i].fatia = dim/nthreads;
        args[i].dim = dim;
        args[i].k = 2.0; // Exemplo de constante k

        if (i == nthreads - 1)
            args[i].fatia += dim % nthreads;

        
    }

    for (int i = 0; i < nthreads; i++)
    {
        if (pthread_create(&threads[i], NULL, somaVetores, (void *)&args[i]) != 0)
        {
            fprintf(stderr, "Erro ao criar a thread %d\n", i);
            return -1;
        }
    }

    // Aguarda o término das threads
    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Imprime o vetor resultante
    for (int i = 0; i < dim; i++)
    {
        printf("%f ", vetorC[i]);
    }
    printf("\n");

    return 0;
}

void *somaVetores(void *arg) 
{
    t_args *args = (t_args *)arg;
    int id = args->id;
    int fatia = args->fatia;
    int dim = args->dim;
    int k = args->k;
    int inicio = id * fatia;
    int fim = inicio + fatia;

    if (id == (args->dim / args->fatia) - 1) // Última thread pode pegar o restante
        fim += dim % args->fatia;

    for (int i = inicio; i < fim; i++)
    {
        vetorC[i] = vetorA[i] * k + vetorB[i];
    }

    pthread_exit(NULL);
}
