/*O programa abaixo foi implementado por um colega em uma atividade de laboratório de uma edição anterior da disciplina. 
O roteiro foi o seguinte:
Implemente um programa com 5 threads:
A thread 1 imprime a frase “Oi Maria!”;
A thread 2 imprime a frase “Oi José!”;
A thread 3 imprime a frase “Sente-se por favor.”;
A thread 4 imprime a frase “Até mais José!”;
A thread 5 imprime a frase “Até mais Maria!”.

As threads devem ser criadas todas de uma vez na função main. As regras de impressão
das mensagens (execução das threads) serão:
(i) A ordem em que as threads 1 e 2 imprimem suas mensagens não importa, mas
ambas devem sempre imprimir suas mensagens antes da thread 3.
(ii) A ordem em que as threads 4 e 5 imprimem suas mensagens não importa, mas
ambas devem sempre imprimir suas mensagens depois da thread 3.
*/

#include<stdio.h>
#include<pthread.h>
#define NTHREADS 5

//variaveis globais
int chegadas = 0;
int sentados = 0;
pthread_mutex_t x_mutex;
pthread_cond_t chegada_cond;
pthread_cond_t sentado_cond;

void *chegada(void *arg) {
    int thread_id = *(int*)arg;
    if (thread_id == 0) 
        printf("Oi José!\n");
    else
        printf("Oi Maria!\n");
    pthread_mutex_lock(&x_mutex);
    chegadas++;
    if (chegadas == 2) {
        pthread_cond_signal(&chegada_cond);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *permanencia(void *arg) {
    pthread_mutex_lock(&x_mutex);
    while (chegadas != 2) {
       pthread_cond_wait(&chegada_cond, &x_mutex);
    }   
    printf("Sentem-se por favor.\n");
    sentados++;
    pthread_cond_broadcast(&sentado_cond); 
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *saida(void *arg) {
    pthread_mutex_lock(&x_mutex);
    while(sentados != 1) 
       pthread_cond_wait(&sentado_cond, &x_mutex);
    pthread_mutex_unlock(&x_mutex);
    int thread_id = *(int*)arg;  
    if (thread_id == 3) 
        printf("Tchau José!\n");
    else
        printf("Tchau Maria!\n");
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&chegada_cond, NULL);
  pthread_cond_init (&sentado_cond, NULL);

  int thread_ids[NTHREADS] = {0, 1, 2, 3, 4};

  pthread_create(&threads[0], NULL, chegada, &thread_ids[0]);
  pthread_create(&threads[1], NULL, chegada, &thread_ids[1]);
  pthread_create(&threads[2], NULL, permanencia, &thread_ids[2]);
  pthread_create(&threads[3], NULL, saida, &thread_ids[3]);
  pthread_create(&threads[4], NULL, saida, &thread_ids[4]);
  
  pthread_exit(NULL);
  return 0;
}
