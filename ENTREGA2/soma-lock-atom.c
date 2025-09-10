/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

long int soma = 0; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t cond_print; // variavel de condicao para impressao
pthread_cond_t cond_continue; // variavel de condicao para continuar
short int flag_imprimir = 0; //flag que indica quando precisamos imprimir
short int threads_ativas = 0; //contador de threads em execução

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  pthread_mutex_lock(&mutex);
  threads_ativas++;
  pthread_mutex_unlock(&mutex);

  for (int i=0; i<100000; i++) {
     pthread_mutex_lock(&mutex);
     long int valor_anterior = soma;
     soma++;
     
     //se após incrementar, soma virou múltiplo de 1000
     if (valor_anterior % 1000 != 0 && soma % 1000 == 0) {
        flag_imprimir = 1;
        //acorda a thread extra para imprimir
        pthread_cond_signal(&cond_print);
     }
     
     //se temos algo para imprimir, todas as threads devem esperar
     if (flag_imprimir) {
        pthread_cond_wait(&cond_continue, &mutex);
     }
     pthread_mutex_unlock(&mutex);
  }

  pthread_mutex_lock(&mutex);
  threads_ativas--;
  if (threads_ativas == 0) {
      pthread_cond_signal(&cond_print);
  }
  pthread_mutex_unlock(&mutex);

  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");
  
  while(1) {
    pthread_mutex_lock(&mutex);
    
    //espera ate ter algo para imprimir ou todas threads terminarem
    while (flag_imprimir == 0 && threads_ativas > 0) {
        pthread_cond_wait(&cond_print, &mutex);
    }
    
    //se nao ha mais threads ativas, terminamos
    if (threads_ativas == 0 && flag_imprimir == 0) {
        pthread_mutex_unlock(&mutex);
        printf("Extra : terminou!\n");
        pthread_exit(NULL);
    }
    
    //se ha algo para imprimir e é múltiplo de 1000
    if (flag_imprimir == 1) {
        if (soma % 1000 == 0) {
            printf("soma = %ld \n", soma);
        }
        flag_imprimir = 0;
        //acorda todas as threads que estão esperando
        pthread_cond_broadcast(&cond_continue);
    }
    
    pthread_mutex_unlock(&mutex);
  }
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--inicilaiza o mutex (lock de exclusao mutua)
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_print, NULL);
   pthread_cond_init(&cond_continue, NULL);

   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex e variaveis de condicao
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_print);
   pthread_cond_destroy(&cond_continue);

   printf("Valor de 'soma' = %ld\n", soma);

   free(tid);
   return 0;
}
