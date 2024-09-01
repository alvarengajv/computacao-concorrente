#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct {
    int idThread, nThreads;
    int inicio, fim;
    float *vetor_a, *vetor_b;  // float *vetor_a, *vetor_b;
    double resultado_parcial;  // float resultado_parcial;
} t_Args;

void* calcular_produto_interno(void* arg) {
    t_Args *args = (t_Args *) arg;
    double soma = 0;  // float soma = 0;

    for (int i = args->inicio; i < args->fim; i++) {
        soma += args->vetor_a[i] * args->vetor_b[i];
    }

    args->resultado_parcial = soma;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <numero_de_threads> <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    int nThreads = atoi(argv[1]);
    char* nome_arquivo = argv[2];

    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    // Ler N e os vetores do arquivo
    int N;
    fread(&N, sizeof(int), 1, arquivo);

    float *vetor_a = (float *)malloc(N * sizeof(float));
    float *vetor_b = (float *)malloc(N * sizeof(float));
    fread(vetor_a, sizeof(float), N, arquivo);
    fread(vetor_b, sizeof(float), N, arquivo);

    double resultado_sequencial;  // float resultado_sequencial;
    fread(&resultado_sequencial, sizeof(double), 1, arquivo);  // fread(&resultado_sequencial, sizeof(float), 1, arquivo);

    fclose(arquivo);

    // Criar as threads
    pthread_t threads[nThreads];
    t_Args args[nThreads];
    int tamanho_bloco = N / nThreads;
    int resto = N % nThreads;

    int inicio = 0;
    for (int i = 0; i < nThreads; i++) {
        args[i].idThread = i + 1;
        args[i].nThreads = nThreads;
        args[i].inicio = inicio;
        args[i].fim = inicio + tamanho_bloco + (i < resto ? 1 : 0);
        args[i].vetor_a = vetor_a;
        args[i].vetor_b = vetor_b;
        args[i].resultado_parcial = 0.0;  // args[i].resultado_parcial = 0.0f;

        pthread_create(&threads[i], NULL, calcular_produto_interno, (void*)&args[i]);

        inicio = args[i].fim;
    }

    // Aguardar as threads terminarem
    double resultado_concorrente = 0.0;  // float resultado_concorrente = 0.0f;
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
        resultado_concorrente += args[i].resultado_parcial;
    }

    // Calcular a variação relativa
    double variacao_relativa = (resultado_sequencial - resultado_concorrente) / resultado_sequencial;
    // float variacao_relativa = (resultado_sequencial - resultado_concorrente) / resultado_sequencial;

    // Exibir os resultados
    printf("Produto interno sequencial: %f\n", resultado_sequencial);
    printf("Produto interno concorrente: %f\n", resultado_concorrente);
    printf("Variação relativa: %f\n", variacao_relativa);

    // Liberar memória
    free(vetor_a);
    free(vetor_b);

    return 0;
}
