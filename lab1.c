/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Laboratório: 1 */
/* Codigo: Incrementando um vetor usando threads em C e aguardando as threads terminarem */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// Cria a estrutura de dados para armazenar os argumentos das threads
typedef struct {
    int idThread;  // Identificador da thread
    int nThreads;  // Número total de threads
    int *vetor;    // Ponteiro para o vetor
    int inicio;    // Índice de início da porção do vetor
    int fim;       // Índice de fim da porção do vetor
} t_Args;

// Função executada pelas threads
void* somar(void* arg) {
    t_Args *args = (t_Args *)arg;
    printf("Thread %d de %d threads: incrementando posições de %d até %d.\n", 
           args->idThread, args->nThreads, args->inicio, args->fim-1);
    for (int i = args->inicio; i < args->fim; i++) {
        args->vetor[i] += 1;
    }
    free(arg); // Libera a alocação feita na main
    pthread_exit(NULL);
}

// Função principal do programa
int main(int argc, char* argv[]) {
    int nthreads, tamanho_vetor; // Quantidade de threads e tamanho do vetor

    // Verifica se os argumentos foram passados e armazena seus valores
    if(argc != 3) {
        printf("--ERRO: informe a qtde de threads e o tamanho do vetor <%s> <nthreads> <tamanho_vetor>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);
    tamanho_vetor = atoi(argv[2]);

    // Aloca o vetor de inteiros
    int *vetor = (int *)malloc(tamanho_vetor * sizeof(int));
    if (vetor == NULL) {
        printf("--ERRO: malloc()\n");
        return 1;
    }

    // Inicializa o vetor com zeros
    for (int i = 0; i < tamanho_vetor; i++) {
        vetor[i] = 0;
    }

    // Identificadores das threads no sistema
    pthread_t tid_sistema[nthreads];

    // Cria as threads
    int tamanho_bloco = tamanho_vetor / nthreads;
    int resto = tamanho_vetor % nthreads;

    int inicio = 0;
    for (int i = 0; i < nthreads; i++) {
        printf("--Aloca e preenche argumentos para thread %d\n", i + 1);
        t_Args *args = malloc(sizeof(t_Args));
        if (args == NULL) {
            printf("--ERRO: malloc()\n");
            return 1;
        }
        args->idThread = i + 1;
        args->nThreads = nthreads;
        args->vetor = vetor;
        args->inicio = inicio;
        args->fim = inicio + tamanho_bloco + (i < resto ? 1 : 0); // Distribui o resto

        // Cria a thread
        printf("--Cria a thread %d\n", i + 1);
        if (pthread_create(&tid_sistema[i], NULL, somar, (void*) args)) {
            printf("--ERRO: pthread_create()\n");
            return 2;
        }

        inicio = args->fim; // Próximo início
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() da thread %d\n", i);
            return 3;
        }
    }

    // Verificar o resultado final
    int sucesso = 1;
    for (int i = 0; i < tamanho_vetor; i++) {
        if (vetor[i] != 1) {
            sucesso = 0;
            break;
        }
    }
    if (sucesso) {
        printf("Sucesso: Todos os elementos foram incrementados corretamente.\n");
    } else {
        printf("Erro: Nem todos os elementos foram incrementados corretamente.\n");
    }

    // Liberar a memória alocada
    free(vetor);

    // Log da função principal
    printf("--Thread principal terminou\n");

    return 0;
}
