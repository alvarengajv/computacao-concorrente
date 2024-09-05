#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Descomentar o define abaixo para imprimir as matrizes geradas no formato texto
#define TEXTO 

// Função para preencher uma matriz com valores float aleatórios
void gerar_matriz(float *matriz, int linhas, int colunas) {
    long long int tam = linhas * colunas;
    srand(time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual
    for (long long int i = 0; i < tam; i++) {
        matriz[i] = (rand() % 1000) * 0.3f; // Gera valores float aleatórios entre 0 e 300
    }
}

// Função para imprimir a matriz no formato texto
void imprimir_matriz(float *matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%lf ", matriz[i * colunas + j]); // Imprime cada elemento com 6 casas decimais
        }
        printf("\n"); // Nova linha após imprimir todas as colunas
    }
}

int main(int argc, char* argv[]) {
    float *matriz1, *matriz2; // Matrizes que serão geradas
    int linhas, colunas; // Dimensões das matrizes (iguais para ambas)
    long long int tam; // Quantidade de elementos em cada matriz
    FILE *descritorArquivo1, *descritorArquivo2; // Descritores dos arquivos de saída
    size_t ret; // Retorno da função de escrita no arquivo de saída

    // Verifica se o número correto de argumentos foi fornecido
    if (argc < 5) {
        fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo_saida_matriz1> <arquivo_saida_matriz2>\n", argv[0]);
        return 1;
    }

    // Converte os argumentos da linha de comando para inteiros
    linhas = atoi(argv[1]);
    colunas = atoi(argv[2]);
    tam = linhas * colunas;

    // Aloca memória para as duas matrizes
    matriz1 = (float*) malloc(sizeof(float) * tam);
    matriz2 = (float*) malloc(sizeof(float) * tam);
    if (!matriz1 || !matriz2) {
        fprintf(stderr, "Erro de alocação da memória das matrizes\n");
        // Libera memória alocada se houve erro
        if (matriz1) free(matriz1);
        if (matriz2) free(matriz2);
        return 2;
    }

    // Preenche as duas matrizes com valores float aleatórios
    gerar_matriz(matriz1, linhas, colunas);
    gerar_matriz(matriz2, linhas, colunas);

    // Imprimir as matrizes na saída padrão, se a diretiva TEXTO estiver definida
    // #ifdef TEXTO
    // printf("Matriz 1:\n");
    // imprimir_matriz(matriz1, linhas, colunas);
    // printf("\nMatriz 2:\n");
    // imprimir_matriz(matriz2, linhas, colunas);
    // #endiclear

    // Escreve a primeira matriz no primeiro arquivo
    descritorArquivo1 = fopen(argv[3], "wb");
    if (!descritorArquivo1) {
        fprintf(stderr, "Erro de abertura do arquivo para a matriz 1\n");
        free(matriz1);
        free(matriz2);
        return 3;
    }
    // Escreve o número de linhas e de colunas
    ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo1);
    ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo1);
    // Escreve os elementos da matriz
    ret = fwrite(matriz1, sizeof(float), tam, descritorArquivo1);
    if (ret < tam) {
        fprintf(stderr, "Erro de escrita no arquivo para a matriz 1\n");
        fclose(descritorArquivo1);
        free(matriz1);
        free(matriz2);
        return 4;
    }
    fclose(descritorArquivo1);

    // Escreve a segunda matriz no segundo arquivo
    descritorArquivo2 = fopen(argv[4], "wb");
    if (!descritorArquivo2) {
        fprintf(stderr, "Erro de abertura do arquivo para a matriz 2\n");
        free(matriz1);
        free(matriz2);
        return 5;
    }
    // Escreve o número de linhas e de colunas
    ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo2);
    ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo2);
    // Escreve os elementos da matriz
    ret = fwrite(matriz2, sizeof(float), tam, descritorArquivo2);
    if (ret < tam) {
        fprintf(stderr, "Erro de escrita no arquivo para a matriz 2\n");
        fclose(descritorArquivo2);
        free(matriz1);
        free(matriz2);
        return 6;
    }
    fclose(descritorArquivo2);

    // Libera a memória alocada para as matrizes
    free(matriz1);
    free(matriz2);
    return 0;
}
