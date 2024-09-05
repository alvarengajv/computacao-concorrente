#include <stdio.h>
#include <stdlib.h>
#include "timer.h" 

// Função para ler uma matriz de um arquivo binário
void ler_matriz(const char *arquivo, float **matriz, int *linhas, int *colunas) {
    FILE *file = fopen(arquivo, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }
    fread(linhas, sizeof(int), 1, file);
    fread(colunas, sizeof(int), 1, file);
    long long int tam = (*linhas) * (*colunas);
    *matriz = (float*) malloc(sizeof(float) * tam);
    if (!*matriz) {
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        exit(2);
    }
    fread(*matriz, sizeof(float), tam, file);
    fclose(file);
}

// Função para escrever uma matriz em um arquivo binário
void escrever_matriz(const char *arquivo, float *matriz, int linhas, int colunas) {
    FILE *file = fopen(arquivo, "wb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivo);
        exit(3);
    }
    fwrite(&linhas, sizeof(int), 1, file);
    fwrite(&colunas, sizeof(int), 1, file);
    long long int tam = linhas * colunas;
    fwrite(matriz, sizeof(float), tam, file);
    fclose(file);
}

// Função para multiplicar duas matrizes
void multiplicar_matrizes(float *matriz1, int linhas1, int colunas1, 
                          float *matriz2, int linhas2, int colunas2, 
                          float **resultado, int *linhas_res, int *colunas_res) {
    if (colunas1 != linhas2) {
        fprintf(stderr, "Dimensões das matrizes não compatíveis para multiplicação\n");
        exit(4);
    }

    *linhas_res = linhas1;
    *colunas_res = colunas2;
    long long int tam_res = (*linhas_res) * (*colunas_res);
    *resultado = (float*) malloc(sizeof(float) * tam_res);
    if (!*resultado) {
        fprintf(stderr, "Erro de alocação da memória para a matriz resultado\n");
        exit(5);
    }

    // Inicializa a matriz resultado com zeros
    for (long long int i = 0; i < tam_res; i++) {
        (*resultado)[i] = 0.0f;
    }

    // Multiplicação de matrizes
    for (int i = 0; i < linhas1; i++) {
        for (int j = 0; j < colunas2; j++) {
            for (int k = 0; k < colunas1; k++) {
                (*resultado)[i * (*colunas_res) + j] += matriz1[i * colunas1 + k] * matriz2[k * colunas2 + j];
            }
        }
    }
}

// Função para imprimir a matriz 
void imprimir_matriz(float *matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%f ", matriz[i * colunas + j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    float *matriz1, *matriz2, *resultado; 
    int linhas1, colunas1, linhas2, colunas2; 
    int linhas_res, colunas_res; 
    double inicio, fim, delta;

    if (argc < 4) {
        fprintf(stderr, "Uso: %s <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    GET_TIME(inicio);

    ler_matriz(argv[1], &matriz1, &linhas1, &colunas1);
    ler_matriz(argv[2], &matriz2, &linhas2, &colunas2);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de leitura: %lf segundos\n", delta);

    // Imprime as dimensões das matrizes para verificar
    // printf("Dimensões da matriz 1: %d x %d\n", linhas1, colunas1);
    // printf("Dimensões da matriz 2: %d x %d\n", linhas2, colunas2);

    // Imprime o conteúdo das matrizes (opcional, para verificar se os valores foram lidos corretamente)
    // printf("Matriz 1:\n");
    // imprimir_matriz(matriz1, linhas1, colunas1);
    // printf("Matriz 2:\n");
    // imprimir_matriz(matriz2, linhas2, colunas2);

    GET_TIME(inicio);

    multiplicar_matrizes(matriz1, linhas1, colunas1, matriz2, linhas2, colunas2, &resultado, &linhas_res, &colunas_res);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento: %lf segundos\n", delta);

    GET_TIME(inicio);

    escrever_matriz(argv[3], resultado, linhas_res, colunas_res);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf segundos\n", delta);

    free(matriz1);
    free(matriz2);
    free(resultado);

    return 0;
}