/* Primeiro programa concorrente usando a biblioteca pthread
7 de agosto de 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//funcao que as threads executam
void *task(void *arg){
    long int id = (long int) arg;
    printf("Oi turma, tudo bem? Sou a thread %ld\n", id);
    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]){
    //declaracao de variaveis
    short int nthreads;

    //receber os argumentos de entrada e valida-los
    if(argc < 2){
        printf("ERRO: digite %s <numero threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    //criar vetor de identificadores de threads
    pthread_t tids[nthreads];

    //criar as threads
    for(long int i = 0; i < nthreads; i++){
        if(pthread_create(&tids[i], NULL, task, (void*) i)){
            printf("ERRO: pthread_create da thread %ld\n", i);
            return 2;
        }
    }

    //esperar as threads terminarem
    for(short int i = 0; i < nthreads; i++){
        if(pthread_join(tids[i], NULL)){
            printf("ERRO: pthread_join da thread %hd\n", i);
            return 3;
        }
    }

    //finalizar a execucao
    puts("Main terminou");
    return 0;
}