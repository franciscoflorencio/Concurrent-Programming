#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h>
#include "timer.h"

float *vetorA;
float *vetorB;

typedef struct {
   long int n;
   short int nthreads;
   short int id;
} t_args;  

void *ProdutoInternoParcial (void *args) {
  t_args *arg = (t_args*) args;
  long int ini, fim;
  long int fatia = arg->n / arg->nthreads;
  double *soma_parcial = (double*) malloc(sizeof(double));
  *soma_parcial = 0.0;
  
  ini = arg->id * fatia;
  fim = (arg->id == arg->nthreads - 1) ? arg->n : ini + fatia;

  for(long int i = ini; i < fim; i++) {
     *soma_parcial += vetorA[i] * vetorB[i];
  }
  pthread_exit((void*) soma_parcial);
}

int main(int argc, char *argv[]) {
  if(argc < 3) { 
    fprintf(stderr, "Use: %s <arquivo> <threads>\n", argv[0]); 
    return 1; 
  }

  long int n;
  short int nthreads = atoi(argv[2]);
  float vs;
  double vc = 0.0;
  double inicio, fim, delta;
  
  FILE *arq = fopen(argv[1], "rb");
  if (!arq) { perror("Erro fopen"); return 2; }

  fread(&n, sizeof(long int), 1, arq);
  vetorA = (float*) malloc(sizeof(float) * n);
  vetorB = (float*) malloc(sizeof(float) * n);
  fread(vetorA, sizeof(float), n, arq);
  fread(vetorB, sizeof(float), n, arq);
  fread(&vs, sizeof(float), 1, arq);
  fclose(arq);

  pthread_t *tid_sistema = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
  
  GET_TIME(inicio);

  t_args* args;
  for(short int i = 0; i < nthreads; i++) {
    args = (t_args*) malloc(sizeof(t_args));
    args->n = n;
    args->nthreads = nthreads;
    args->id = i;
    pthread_create(&tid_sistema[i], NULL, ProdutoInternoParcial, (void*) args);
  }

  double *retorno_thread;
  for(short int i = 0; i < nthreads; i++) {
     pthread_join(tid_sistema[i], (void**) &retorno_thread);
     vc += *retorno_thread;
     free(retorno_thread);
  }
  
  GET_TIME(fim);
  delta = fim - inicio;

  float variacao = fabsf(vs - vc) / fabsf(vs);

  printf("TEMPO: %lf\n", delta);
  printf("VARIACAO: %.12f\n", variacao);

  free(vetorA);
  free(vetorB);
  free(tid_sistema);

  return 0;
}