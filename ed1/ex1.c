#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct 
{
    double *partial_sum;
    int start;
    int end;
} t_args;

void *thread_pi_approx(void *arg)
{
    t_args* args = (t_args*) arg;
    double local_pi = 0.0;

    for (int i = args->start; i < args->end; i++)
    {
        local_pi += ( (4.0/(8*i+1)) - (2.0/(8*i+4)) - (1.0/(8*i+5)) - (1.0/(8*i+6)) ) * (1.0/pow(16, i));
    }

    *(args->partial_sum) = local_pi;
    pthread_exit(NULL);
}

void pi_approx(double *pi, int n)
{
    *pi = 0.0;
    for (int i = 0; i < n; i++)
    {
        *pi += ( (4.0/(8*i+1)) - (2.0/(8*i+4)) - (1.0/(8*i+5)) - (1.0/(8*i+6)) ) * (1.0/pow(16, i));
    }
}

int main()
{
    int n = 100000; // numero de iteracoes

    // PARTE SEQUENCIAL DO PROGRAMA
    double *pi = (double *)malloc(sizeof(double));
    pi_approx(pi, n);

    printf("pi aproximado em n = 100000 sem threads: ");
    printf("%lf\n", *pi);

    free(pi);


    // PARTE CONCORRENTE DO PROGRAMA
    int M = 4; // numero de threads

    pthread_t *threads = (pthread_t*)malloc(M*sizeof(pthread_t));
    t_args *args = (t_args*)malloc(M*sizeof(t_args));
    double *partial_sums = (double*)malloc(M*sizeof(double));

    int chunck_size = n / M;

    // cria M threads com workload balanceado
    for (int i = 0; i < M; i++)
    {
        args[i].partial_sum = &partial_sums[i];
        args[i].start = i * chunck_size;
        args[i].end = (i == M - 1) ? n : (i + 1) * chunck_size;

        pthread_create(&threads[i], NULL, thread_pi_approx, &args[i]);
    }

    // espera pelo fim de cada uma das threads
    double final_pi = 0.0;
    for (int i = 0; i < M; i++)
    {
        pthread_join(threads[i], NULL);
        final_pi += partial_sums[i];
    }

    printf("\n\npi aproximado em n = 100000 com %d threads: ", M);
    printf("%lf\n", final_pi);

    free(threads);
    free(args);
    free(partial_sums);

    return 0;
}