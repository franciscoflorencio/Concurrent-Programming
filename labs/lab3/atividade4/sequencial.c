#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerarVetor(float* vetor, long n) {
    for (long i = 0; i < n; i++) {
        vetor[i] = ((float)rand() / RAND_MAX) * 10.0;
    }
}

double calcularProdutoInterno(float* vetorA, float* vetorB, long n) {
    double produto = 0.0;
    for (long i = 0; i < n; i++) {
        produto += vetorA[i] * vetorB[i];
    }
    return produto;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <dimensao_N>\n", argv[0]);
        return 1;
    }

    long N = atol(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Erro: A dimensao deve ser um numero positivo.\n");
        return 1;
    }

    float* vetorA = (float*)malloc(N * sizeof(float));
    float* vetorB = (float*)malloc(N * sizeof(float));
    if (vetorA == NULL || vetorB == NULL) {
        perror("Erro de alocacao");
        return 2;
    }

    srand(time(NULL));
    gerarVetor(vetorA, N);
    gerarVetor(vetorB, N);

    float produtoInterno = (float)calcularProdutoInterno(vetorA, vetorB, N);

    FILE* arquivo = fopen("dados_vetores.bin", "wb");
    if (arquivo == NULL) {
        perror("Erro ao criar arquivo binario");
        return 3;
    }
    
    fwrite(&N, sizeof(long), 1, arquivo);
    fwrite(vetorA, sizeof(float), N, arquivo);
    fwrite(vetorB, sizeof(float), N, arquivo);
    fwrite(&produtoInterno, sizeof(float), 1, arquivo);
    
    fclose(arquivo);
    free(vetorA);
    free(vetorB);

    return 0;
}