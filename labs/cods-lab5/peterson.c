#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

// Variáveis compartilhadas para o algoritmo de Peterson
bool flag[2] = {false, false};  // Indica se um processo quer entrar na região crítica
int turn = 0;                   // De quem é a vez
int shared_resource = 0;        // Recurso compartilhado

void* process(void* arg) {
    int id = *(int*)arg;        // ID do processo (0 ou 1)
    int other = 1 - id;         // ID do outro processo

    for (int i = 0; i < 5; i++) {
        // Protocolo de entrada
        flag[id] = true;        // Indica intenção de entrar na região crítica
        turn = other;           // Dá a vez para o outro processo
        
        // Espera enquanto o outro processo quer entrar e é sua vez
        while (flag[other] && turn == other) {
            // Espera ocupada
        }

        // Região crítica
        printf("Processo %d entrou na região crítica\n", id);
        shared_resource++;
        printf("Processo %d: shared_resource = %d\n", id, shared_resource);
        sleep(1);  // Simula algum trabalho
        printf("Processo %d saiu da região crítica\n", id);

        // Protocolo de saída
        flag[id] = false;       // Indica que terminou

        // Região não crítica
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t threads[2];
    int ids[2] = {0, 1};

    // Cria as threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }

    // Espera as threads terminarem
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Valor final do recurso compartilhado: %d\n", shared_resource);
    return 0;
}
