#include <semaphore.h>

sem_t em_e, em_l, escr, leit;
int e = 0, l = 0; // e = escritores esperando/escrevendo, l = leitores lendo

void init() {
    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);
}

void AntesLeitores() {
    sem_wait(&leit);
    sem_wait(&em_l);

    l++;
    if (l == 1) sem_wait(&escr);

    sem_post(&em_l);
    sem_post(&leit);
}

void DepoisLeitores() {
    sem_wait(&em_l);

    l--;
    if (l == 0) sem_post(&escr);

    sem_post(&em_l);
}

void AntesEscritores() {
    sem_wait(&em_e);

    e++;
    if (e == 1) sem_wait(&leit);

    sem_post(&em_e);
    sem_wait(&escr);
}

void DepoisEscritores() {
    sem_post(&escr);
    sem_wait(&em_e);

    e--;
    if (e == 0) sem_post(&leit);

    sem_post(&em_e);
}