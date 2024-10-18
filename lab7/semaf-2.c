/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: As tres threads executam de acordo com as transições de estado (t1-t2-t3) */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3

// Variaveis globais
sem_t estado1, estado2;      //semaforos para coordenar a ordem de execucao das threads

//funcao executada pela thread 1
void *t1 (void *arg) {
   printf("Thread : 1 esta executando...\n");
   
   printf("Thread : 1 mudou estado!\n");
   //faz qq coisa ate mudar de estado
   sem_post(&estado1); 
   
   printf("Thread : 1 terminou!\n");
   pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *arg) {
   printf("Thread : 2 esta executando...\n");

   //espera a transicao para o estado 1
   sem_wait(&estado1); 
   printf("Thread : 2 mudou estado!\n");
   
   //faz qq coisa ate mudar de estado
   sem_post(&estado2); 
   
   printf("Thread : 2 terminou!\n");
   pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *arg) {
   printf("Thread : 3 esta executando...\n");

   //espera a transicao para o estado 2
   sem_wait(&estado2); 
   printf("Thread : 3 mudou estado!\n");
   //faz qq coisa
   
   printf("Thread : 3 terminou!\n");
   pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];

  //inicia os semaforos
  sem_init(&estado1, 0, 0);
  sem_init(&estado2, 0, 0);

  //cria as tres threads
  if (pthread_create(&tid[2], NULL, t3, NULL)) { 
	  printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[1], NULL, t2, NULL)) { 
	  printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[0], NULL, t1, NULL)) { 
	  printf("--ERRO: pthread_create()\n"); exit(-1); }

  //--espera todas as threads terminarem
  for (int t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  } 

  return 0;
}
