#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int nthreads;
int termos;

void *somatorioSerio(void *arg){
    int id = (int) arg;
    double *soma;
    soma = (double*)malloc(sizeof(double));
    if(soma == NULL) return -1;
    *soma = 0;

    for(int i = id; i<termos; i+=nthreads){
        int aux = 8*i; 
        *soma += ((4.0/(aux+1) - 2.0/(aux+4) - 1/(aux+5) - 1/(aux+6))*(1/pow(16,i)));
    }

    pthread_exit(NULL);
}