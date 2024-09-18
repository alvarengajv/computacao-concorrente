#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; //variavel compartilhada entre as threads
int impressos = 0; //contador de multiplos de 10 
int pode_imprimir = 0; //flag para controlar quando o valor esta pronto para ser impresso
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t cond; //variavel condicional para sincronizacao

//funcao executada pelas threads
void *ExecutaTarefa(void *arg) {
    long int id = (long int)arg;
    printf("Thread : %ld esta executando...\n", id);

    for (int i = 0; i < 100000; i++) {
        //--entrada na SC
        pthread_mutex_lock(&mutex);
        //--SC (seção critica)
        soma++; //incrementa a variavel compartilhada 

        // Se soma for múltiplo de 10, notifica a thread extra e aguarda
        if (soma % 10 == 0 && impressos < 20) {
            pode_imprimir = 1;
            pthread_cond_signal(&cond); // sinaliza para a thread extra
            while (pode_imprimir == 1) {
                pthread_cond_wait(&cond, &mutex);
            }
        }

        //--saida da SC
        pthread_mutex_unlock(&mutex);
    }
    printf("Thread : %ld terminou!\n", id);
    pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra(void *args) {
    printf("Extra : esta executando...\n");
    while (impressos < 20) {
        pthread_mutex_lock(&mutex);

        // espera até que um múltiplo de 10 esteja pronto para ser impresso
        while (pode_imprimir == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        // imprime o valor de soma
        if (soma % 10 == 0) {
            printf("soma = %ld \n", soma);
            impressos++;
        }

        // libera a thread ExecutaTarefa para continuar
        pode_imprimir = 0;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
    printf("Extra : terminou!\n");
    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
    pthread_t tid; //identificador da thread no sistema
    pthread_t tid_extra; //identificador da thread de log

    //--inicilaiza o mutex e a variavel condicional
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //--cria a thread
    if (pthread_create(&tid, NULL, ExecutaTarefa, (void*)1)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    //--cria thread de log
    if (pthread_create(&tid_extra, NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    //--espera a thread terminar
    if (pthread_join(tid, NULL)) {
        printf("--ERRO: pthread_join() \n");
        exit(-1);
    }

    //--espera a thread de log terminar
    if (pthread_join(tid_extra, NULL)) {
        printf("--ERRO: pthread_join() \n");
        exit(-1);
    }

    //--finaliza o mutex e a variavel condicional
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Valor de 'soma' = %ld\n", soma);

    return 0;
}