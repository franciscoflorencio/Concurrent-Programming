#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    short int id; //identificador da thread na aplicaco
    short int nthreads; //quantidade de threads
    long int dim; //dimensao do vetor
} t_args;

int *vetor; //vetor que será processado

//funcao executada pelas thread
void *f(void *args)
{
    t_args *arg = (t_args*) args;
    long int fatia, ini, fim;
    fatia = arg->dim / arg->nthreads; //quantidade de elementos que a thread vai processar
    ini = arg->id * fatia;
    fim = ini + fatia;

    if (arg->id == arg->nthreads-1)
        fim = arg->dim;

    for(long int i = ini; i < fim; i++)
    {
        *(vetor+i) += 1;
    }

    free(args); //libera a memoria alocada na main
    pthread_exit(NULL);
}

//funcao para alocar espaco e inicializar o vetor
short int inicializar_vetor(int **vet, long int dim)
{
    *vet = (int*) malloc(sizeof(int) * dim);

    if (*vet == NULL)
    {
        fprintf(stderr, "ERRO de alocacao de memoria\n");
        return -1;
    }

    //inicializa o vetor
    for (long int i = 0; i < dim; i++)
    {
        *(*vet+i) = (int)i;
    }
    return 0; //0 significa sucesso
}

//funcao que exibe o vetor
void imprime_vetor(int *vet, long int dim)
{
    
    for (long int i = 0; i < dim; i++)
    {
        printf("%d ", *(vet+i));
        puts("");
    }
}

//funcao que verifica o vetor saida
short int verifica_vetor(int *vet, long int dim)
{
    for (long int i = 0; i < dim; i++)
        if (*(vetor+i) != i+1) return 1;
    return 0;
}

//funcao principal
int main(int argc, char *argv[])
{
    //declaracao de variaveis
    short int nthreads; //quantidade de threads de execucao
    long int dim; //dimensao do vetor de entrada

    //validar e receber os argumentos de entrada
    if (argc < 3)
    {
        fprintf(stderr, "ERRO de entrada, digite: <%s> <dimensao do vetor> <quantidade de threads> \n", argv[0]);
        return 1;
    }

    //inicializar as variaveis
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    pthread_t tid[nthreads]; //identificadores das threads no sistema

    //inicializar o vetor de processamento
     if (inicializar_vetor(&vetor, dim))
     {
        fprintf(stderr, "ERRO de inicializacao do vetor\n");
        return 2;
     }

    //criar as threads
    for (short int i = 0; i < nthreads; i++)
    {
        //carregar os argumentos para threads
        t_args *args = (t_args *) malloc(sizeof(t_args));
        if (args == NULL)
        {
            fprintf(stderr, "ERRO de criacao da estrutura de argumentos\n");
            return 3;
        }
        args->id = i;
        args->nthreads = nthreads;
        args->dim = dim;

        if(pthread_create(&tid[i], NULL, f, (void *) args))
        {
            fprintf(stderr, "ERRO de criacao da thread %hd\n", i);
            return 4;

        }
    }

    //esperar as threads terminarem
    for (short int i = 0; i < nthreads; i++)
    {
        if (pthread_join(tid[i], NULL))
        {
            fprintf(stderr, "ERRO na espera de threads");
            return 5;
        }
    }

    //verificar o resultado do processamento
    imprime_vetor(vetor, dim);

    if(verifica_vetor(vetor, dim))
        fprintf(stderr, "ERRO de processamento do vetor\n");
    else
        fprintf(stderr, "tudo ok!\n");

    //finalizar
    return 0;
}

