#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    short id;
    short nthreads;
    int fluxos;
} t_args;

double *piParalelo(long long n);
double piSequencial(long long n);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Utilizacao: %s <numero de termos>\n", argv[0]);
        return 1;
    }

    long long n = atoll(argv[1]);

    double pi_seq = piSequencial(n);
    printf("Valor de pi (sequencial): %.15f\n", pi_seq);

    double *pi_par = piParalelo(n);
    printf("Valor de pi (paralelo): %.15f\n", *pi_par);
    free(pi_par);

    return 0;
}

double piSequencial(long long n)
{
    double soma = 0.0, fator = 1.0;
    long long i;
    for (i = 0; i < n; i++)
    {
        soma = soma + fator / (2 * i + 1);
        fator = -fator;
    }

    return 4.0 * soma;
}

double *piParalelo(long long n)
{
    
}