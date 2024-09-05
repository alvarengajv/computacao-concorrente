#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matriz1, *matriz2, *resultado;
int colunas1, colunas2, colunas_res, nthreads; // Definido globalmente

// Estrutura para passar parâmetros para a função da thread
typedef struct {
    int id;  // identificador do bloco de linhas que a thread vai processar
    int dim; // dimensão total das linhas a serem processadas
    int linhas1; // Número de linhas da primeira matriz
    int colunas1; // Número de colunas da primeira matriz
    int linhas2; // Número de linhas da segunda matriz
    int colunas2; // Número de colunas da segunda matriz
    float *resultado; // Matriz resultado
} tArgs;

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

// Função para multiplicar matrizes de forma concorrente
void *multiplicar_matrizes_concorrente(void *arg) {
    tArgs *args = (tArgs*) arg;
    int id = args->id;
    int dim = args->dim;
    
    // Verifica se as dimensões são compatíveis para multiplicação
    if (args->colunas1 != args->linhas2) {
        fprintf(stderr, "Dimensões das matrizes não compatíveis para multiplicação\n");
        pthread_exit(NULL);
    }
    
    // Aloca memória para a matriz resultado se não estiver alocada
    if (args->resultado == NULL) {
        args->resultado = (float*) malloc(sizeof(float) * args->linhas1 * args->colunas2);
        if (!args->resultado) {
            fprintf(stderr, "Erro de alocação da memória para a matriz resultado\n");
            pthread_exit(NULL);
        }
    }
    
    int linhas_por_thread = dim / nthreads;
    int inicio = id * linhas_por_thread;
    int fim = (id == nthreads - 1) ? dim : (id + 1) * linhas_por_thread;
    
    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < args->colunas2; j++) {
            for (int k = 0; k < args->colunas1; k++) {
                args->resultado[i * args->colunas2 + j] += matriz1[i * args->colunas1 + k] * matriz2[k * args->colunas2 + j];
            }
        }
    }
    return NULL;
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
    int i;
    pthread_t *threads;
    tArgs *thread_data;
    int linhas1, linhas2, linhas_res;
    double inicio, fim, delta;

    if (argc < 5) {
        fprintf(stderr, "Uso: %s <matriz1.bin> <matriz2.bin> <resultado.bin> <num_threads>\n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[4]);
    if (nthreads <= 0) {
        fprintf(stderr, "Número de threads deve ser um inteiro positivo.\n");
        return 2;
    }

    threads = (pthread_t*) malloc(nthreads * sizeof(pthread_t));
    thread_data = (tArgs*) malloc(nthreads * sizeof(tArgs));

    GET_TIME(inicio);

    ler_matriz(argv[1], &matriz1, &linhas1, &colunas1);
    ler_matriz(argv[2], &matriz2, &linhas2, &colunas2);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de leitura: %lf segundos\n", delta);

    colunas_res = colunas2;
    linhas_res = linhas1;

    // Aloca memória para o resultado aqui
    resultado = (float*) malloc(sizeof(float) * linhas_res * colunas_res);
    if (!resultado) {
        fprintf(stderr, "Erro de alocação da memória para a matriz resultado\n");
        return 4;
    }

    // Inicializa a matriz resultado com zeros
    for (long long int i = 0; i < linhas_res * colunas_res; i++) {
        resultado[i] = 0.0f;
    }

    GET_TIME(inicio);

    // Divide o trabalho entre as threads
    for (i = 0; i < nthreads; i++) {
        thread_data[i].id = i;
        thread_data[i].dim = linhas1; // Define a dimensão total para todas as threads
        thread_data[i].linhas1 = linhas1;
        thread_data[i].colunas1 = colunas1;
        thread_data[i].linhas2 = linhas2;
        thread_data[i].colunas2 = colunas2;
        thread_data[i].resultado = resultado; // Passa o ponteiro para a matriz resultado
        pthread_create(&threads[i], NULL, multiplicar_matrizes_concorrente, (void*) &thread_data[i]);
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

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
    free(threads);
    free(thread_data);

    return 0;
}