#include <stdio.h>
#include <stdlib.h>
#define N 3

void multiplicao_ingenua(float** a, float **b, float **c)
{
    float soma;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            soma = 0.0;
            for (int k = 0; k < N; k++)
            {
                soma += a[i][k] * b[k][j];
            }
            c[i][j] = soma;
        }
    }
}