#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

sem_t mutex; 
sem_t empty; // slots vazios do buffer
sem_t full; // slots cheios do buffer

typedef struct {
    int thread_id;
    int prime_count;
} consumer_result;


typedef struct {
    long long int n;
    int m;
    int *buffer;
    int *in; // índice para inserir no buffer
    int *out; // índice para remover do buffer
    consumer_result *results;
    int num_consumers;
    int thread_id; 
} t_args;

int ehPrimo(long long int n){
    int i;
        if (n<=1) return 0;
        if (n==2) return 1;
        if (n%2==0) return 0;
        for (i=3; i<sqrt(n)+1; i+=2)
            if (n%i==0) return 0;
        return 1;
}

void *produtora(void *arg){
    t_args *args = (t_args *)arg;
    long long int n = args->n;
    int m = args->m;
    int *buffer = args->buffer;
    int *in = args->in;
    
    for (long long int i = 0; i < n; i++){
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[*in] = i;
        *in = (*in + 1) % m;

        sem_post(&mutex);
        sem_post(&full);
    }

    for (int i = 0; i < args->num_consumers; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[*in] = n;  
        *in = (*in + 1) % m;

        sem_post(&mutex);
        sem_post(&full);
    }

    pthread_exit(NULL);
}

void *consumidora(void *arg){
    t_args *args = (t_args *)arg;
    int m = args->m;
    int *buffer = args->buffer;
    int *out = args->out;
    int thread_id = args->thread_id; 
    int prime_count = 0;

    while(1) {
        sem_wait(&full);
        sem_wait(&mutex);

        int num = buffer[*out];
        *out = (*out + 1) % m;

        sem_post(&mutex);
        sem_post(&empty);

        if (num == args->n) {
            args->results[thread_id].prime_count = prime_count;
            args->results[thread_id].thread_id = thread_id;
            break;
        }

        if(ehPrimo(num)){
            prime_count++;
        }
    }

    pthread_exit(NULL);
}

void *vencedora(void *arg){
    t_args *args = (t_args *)arg;
    int max_primes = -1;
    int winner_id = -1;

    for (int i = 0; i < args->num_consumers; i++){
        if (args->results[i].prime_count > max_primes){
            max_primes = args->results[i].prime_count;
            winner_id = args->results[i].thread_id;
        }
    }

    printf("Thread vencedora: %d com %d números primos\n", winner_id, max_primes);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Utilizar: %s <numeros inteiros> <canal de inteiros>\n", argv[0]);
        return 1;
    }

    long long int n = atoll(argv[1]);
    int m = atoi(argv[2]);
    if (n <= 0 || m <= 0){
        fprintf(stderr, "Ambos os argumentos precisam ser inteiros positivos!\n");
        return 1;
    }

    if (m > n) {
        fprintf(stderr, "O tamanho do buffer (M=%d) não pode ser maior que o número de elementos (N=%lld)\n", m, n);
        return 1;
    }

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, m);
    sem_init(&full, 0, 0);

    // aqui crio o buffer compartilhado e os índices correspondentes ao mesmo
    int *buffer = malloc(m * sizeof(int));
    int in = 0, out = 0;

    int num_consumers = 4; // valor inteiro arbitrário e ajustável
    pthread_t *cons_threads = malloc(num_consumers * sizeof(pthread_t));
    consumer_result *results = malloc(num_consumers * sizeof(consumer_result));

    t_args args = {
        .n = n,
        .m = m,
        .buffer = buffer,
        .in = &in,
        .out = &out,
        .results = results,
        .num_consumers = num_consumers
    };

    // cria uma thread produtora!
    pthread_t prod_thread;
    pthread_create(&prod_thread, NULL, produtora, &args);

    // cria as threads consumidoras!
    t_args *thread_args = malloc(num_consumers * sizeof(t_args));
    for (int i = 0; i < num_consumers; i++){
        thread_args[i] = args;
        thread_args[i].thread_id = i;
        pthread_create(&cons_threads[i], NULL, consumidora, &thread_args[i]);
    }

    // espera pela thread produtora e consumidoras primeiro
    pthread_join(prod_thread, NULL);
    for (int i = 0; i < num_consumers; i++){
        pthread_join(cons_threads[i], NULL);
    }

    // só cria e executa a thread vencedora depois que todas as outras terminaram
    pthread_t win_thread;
    pthread_create(&win_thread, NULL, vencedora, &args);
    pthread_join(win_thread, NULL);

    //DESTRUIR TUDO (limpeza)
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(buffer);
    free(cons_threads);
    free(results);
    free(thread_args);
    
    return 0;
}