// Atividade 3
// Objetivo: Implementar o padrao leitores e escritores com  ̃ prioridade para escrita.
// Roteiro: Agora e sua vez! Implemente sua vers  ́ ao de  ̃ rwlock com prioridade para
// operac ̧oes de escrita  ̃ . Isso significa que sempre que uma operac ̧ao de escrita for solici-  ̃
// tada, novas operac ̧oes de leitura n  ̃ ao poder  ̃ ao comec ̧ar (mesmo que outras operac ̧  ̃ oes de  ̃
// leitura ja estejam acontecendo), at  ́ e que a operac ̧  ́ ao de escrita seja atendida.  ̃

// 1. Acrescente mensagens de log no seu programa de forma a demonstrar que a pri-
// oridade de escrita esta sendo atendida nas execuc ̧  ́ oes.  ̃

// 2. Execute o programa de teste varias vezes e avalie os resultados.  ́

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 10000000 // quantidade de operacoes sobre a lista
#define QTDE_INI 100 // quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 // valor maximo a ser inserido

struct list_node_s* head_p = NULL; 
int nthreads;

// rwlock de exclusao mutua
pthread_rwlock_t rwlock;
int writing = 0; // Indica se há uma operação de escrita em andamento
pthread_mutex_t writing_mutex = PTHREAD_MUTEX_INITIALIZER;

// Arrays para acumular os resultados de cada thread
int *inserts, *deletes, *reads;

void* tarefa(void* arg) {
    long int id = (long int) arg;
    int op;
    inserts[id] = deletes[id] = reads[id] = 0; // Inicializa contadores

    for(long int i = id; i < QTDE_OPS; i += nthreads) {
        op = rand() % 100;

        if(op < 98) { // Operação de leitura
            pthread_mutex_lock(&writing_mutex);
            if (writing > 0) { // Se houver uma escrita em andamento, bloqueia leitura
                printf("Thread %ld: Leitura bloqueada por operação de escrita.\n", id);
                pthread_mutex_unlock(&writing_mutex);
                continue; // Ignora leitura se houver escrita
            }
            printf("Thread %ld: Leitura permitida.\n", id);
            pthread_mutex_unlock(&writing_mutex);

            pthread_rwlock_rdlock(&rwlock);
            Member(i % MAX_VALUE, head_p); // Ignore return value
            pthread_rwlock_unlock(&rwlock);
            reads[id]++;
        } else if(98 <= op && op < 99) { // Operação de inserção
            pthread_mutex_lock(&writing_mutex);
            writing++; // Indica que uma escrita está em andamento
            pthread_mutex_unlock(&writing_mutex);

            printf("Thread %ld: Iniciando escrita...\n", id);
            pthread_rwlock_wrlock(&rwlock);
            Insert(i % MAX_VALUE, &head_p); // Ignore return value
            pthread_rwlock_unlock(&rwlock);

            pthread_mutex_lock(&writing_mutex);
            writing--; // Finaliza a escrita
            pthread_mutex_unlock(&writing_mutex);
            printf("Thread %ld: Escrita concluída.\n", id);
            inserts[id]++;
        } else if(op >= 99) { // Operação de remoção
            pthread_mutex_lock(&writing_mutex);
            writing++; // Indica que uma escrita está em andamento
            pthread_mutex_unlock(&writing_mutex);

            printf("Thread %ld: Iniciando remoção...\n", id);
            pthread_rwlock_wrlock(&rwlock);
            Delete(i % MAX_VALUE, &head_p); // Ignore return value
            pthread_rwlock_unlock(&rwlock);

            pthread_mutex_lock(&writing_mutex);
            writing--; // Finaliza a escrita
            pthread_mutex_unlock(&writing_mutex);
            printf("Thread %ld: Remoção concluída.\n", id);
            deletes[id]++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t *tid;
    double ini, fim, delta;

    if(argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
    }
    nthreads = atoi(argv[1]);

    // Inicializa os arrays para contar as operações de cada thread
    inserts = malloc(sizeof(int) * nthreads);
    deletes = malloc(sizeof(int) * nthreads);
    reads = malloc(sizeof(int) * nthreads);

    for(int i = 0; i < QTDE_INI; i++)
        Insert(i % MAX_VALUE, &head_p); // Ignore return value

    tid = malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) {
        printf("--ERRO: malloc()\n"); return 2;
    }

    GET_TIME(ini);
    pthread_rwlock_init(&rwlock, NULL);

    // Criação das threads
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, tarefa, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); return 3;
        }
    }

    // Espera todas as threads terminarem
    for(int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), NULL)) {
            printf("--ERRO: pthread_join()\n"); return 4;
        }
    }

    GET_TIME(fim);
    delta = fim - ini;

    // Imprime o resultado acumulado de cada thread
    for (int i = 0; i < nthreads; i++) {
        printf("Thread %d: in=%d out=%d read=%d\n", i, inserts[i], deletes[i], reads[i]);
    }

    // Imprime o tempo total
    printf("Tempo: %lf\n", delta);

    // Limpeza
    pthread_rwlock_destroy(&rwlock);
    free(tid);
    free(inserts);
    free(deletes);
    free(reads);
    Free_list(&head_p);

    return 0;
}