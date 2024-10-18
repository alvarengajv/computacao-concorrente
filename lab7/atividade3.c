#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define N 20  // Tamanho do buffer

char buffer1[N];
char buffer2[N];
sem_t sem_leitura, sem_processamento, sem_impressao;
int fim_leitura = 0;
int fim_processamento = 0;

void *leitura_arquivo(void *arg) {
    FILE *arquivo = (FILE *)arg;
    int lidos;

    while (!fim_leitura) {
        sem_wait(&sem_leitura);
        lidos = fread(buffer1, sizeof(char), N, arquivo);
        if (lidos < N) {
            fim_leitura = 1;
            buffer1[lidos] = '\0';
        }
        sem_post(&sem_processamento);
    }

    pthread_exit(NULL);
}

void *processa_caracteres(void *arg) {
    int count = 0, i, pos = 0;
    int linha_atual = 1;
    int caracteres_na_linha = 1;
    int fase = 0;  // 0 para fase crescente, 1 para fase fixa

    while (!fim_processamento) {
        sem_wait(&sem_processamento);
        pos = 0;

        for (i = 0; i < N && buffer1[i] != '\0'; i++) {
            if (fim_processamento) break;

            buffer2[pos++] = buffer1[i];
            count++;

            if (count == caracteres_na_linha) {
                buffer2[pos++] = '\n';
                count = 0;
                linha_atual++;

                if (fase == 0) {
                    if (linha_atual <= 11) {
                        caracteres_na_linha += 2;
                    } else {
                        fase = 1;
                        caracteres_na_linha = 10;
                    }
                } else {
                    if (linha_atual > 21) {
                        fim_processamento = 1;
                        break;
                    }
                }
            }

            if (pos >= N - 1) {
                break;
            }
        }

        buffer2[pos] = '\0';
        sem_post(&sem_impressao);

        if (fim_leitura && buffer1[0] == '\0') {
            break;
        }
    }

    pthread_exit(NULL);
}

void *imprime_caracteres(void *arg) {
    while (!fim_processamento || buffer2[0] != '\0') {
        sem_wait(&sem_impressao);
        printf("%s", buffer2);
        fflush(stdout);
        sem_post(&sem_leitura);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t tid[3];
    FILE *arquivo_entrada;

    if (argc < 2) {
        printf("Uso: %s <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    arquivo_entrada = fopen(argv[1], "r");
    if (!arquivo_entrada) {
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    sem_init(&sem_leitura, 0, 1);
    sem_init(&sem_processamento, 0, 0);
    sem_init(&sem_impressao, 0, 0);

    pthread_create(&tid[0], NULL, leitura_arquivo, (void *)arquivo_entrada);
    pthread_create(&tid[1], NULL, processa_caracteres, NULL);
    pthread_create(&tid[2], NULL, imprime_caracteres, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem_leitura);
    sem_destroy(&sem_processamento);
    sem_destroy(&sem_impressao);

    fclose(arquivo_entrada);

    return 0;
}