/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

// Variaveis globais
long int soma = 0;    //variavel compartilhada entre as threads
sem_t em;             // semaforo para exclusao mutua entre as threads

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
   long int id = (long int) arg;
   printf("Thread : %ld esta executando...\n", id);
   for (int i=0; i<100000; i++) {
     //--entrada na SC
     sem_wait(&em); 
     //--SC (seção critica)
     soma++; //incrementa a variavel compartilhada 
     //--saida da SC
     sem_post(&em);
   }
   printf("Thread : %ld terminou!\n", id);
   pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

  //  --inicilaiza o mutex (lock de exclusao mutua)
  // sem_init(&em, 0, 1);
  sem_init(&em, 0, 2);

   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
     }
   }

   //--finaliza o semaforo
   sem_destroy(&em);

   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
