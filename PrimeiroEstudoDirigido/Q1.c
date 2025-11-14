#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define M 4

typedef struct 
{
    int start;
    int end;
    double result;
} thread_arg;

double pi_sequencial(int n)
{
    double pi = 0.0;
    for (int i = 0; i < n; i++)
    {
        pi += ((4.0/(8*i + 1)) - (2.0/(8*i + 4)) - (1.0/(8*i + 5)) - (1.0/(8*i + 6))) * (1.0/pow(16, i));
    }

    return pi;
}

void *pi_concorrente(void* arg)
{
    thread_arg *args = (thread_arg*)arg;
    args->result = 0.0;

    for (int i = args->start; i < args->end; i++)
    {
        args->result += ((4.0/(8*i + 1)) - (2.0/(8*i + 4)) - (1.0/(8*i + 5)) - (1.0/(8*i + 6))) * (1.0/pow(16, i));
    }

    pthread_exit(NULL);
}

int main()
{

    int numero_iteracoes = 30;

    double seq = pi_sequencial(numero_iteracoes);
    printf("%lf\n", seq);

    pthread_t threads[M];
    thread_arg args[M];
    int parte_thread = numero_iteracoes/M;
    double pi_paralelo = 0.0;

    int i;
    for (i = 0; i < M; i++)
    {
        args[i].start = i * parte_thread;
        args[i].end = (i+1)*parte_thread;
        pthread_create(&threads[i], NULL, pi_concorrente, &args[i]);
    }

    for (i = 0; i < M; i++)
    {
        pthread_join(threads[i], NULL);
        pi_paralelo += args[i].result;
    }

    printf("Resultado paralelo: %lf\n", pi_concorrente);
    return 0;
}