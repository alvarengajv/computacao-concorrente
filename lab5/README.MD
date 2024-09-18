# Laboratório: 3
Disciplina: Programacao Concorrente 

Profa.: Silvana Rossetto 

João Vitor Lopes Alvarenga - 120152276


# Instruções

## Acessar pasta
cd lab5

### Atividade 2
1. Os requisitos foram atendidos

2. Logs
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ gcc -o exemplo_log exemplo_log.c -pthread
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ ./exemplo_log
[LOG] Criando as threads.
[LOG] Thread de chegada iniciada.
[LOG] Thread de permanência iniciada, aguardando threads de chegada.
[LOG] Thread de chegada iniciada.
Oi Maria!
Oi José!
[LOG] Thread de saída iniciada, aguardando sinal da permanência.
[LOG] Thread de saída iniciada, aguardando sinal da permanência.
[LOG] Chegada de uma thread completa.
[LOG] Chegada de uma thread completa.
[LOG] Ambas as threads de chegada finalizaram. Sinalizando para a thread de permanência.
Sentem-se por favor.
[LOG] Thread de permanência finalizada. Sinalizando para as threads de saída.
Tchau Maria!
[LOG] Thread de saída finalizou sua execução.
Tchau José!
[LOG] Thread de saída finalizou sua execução.

### Atividade 4

joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ gcc -o atividade4 atividade4.c
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ ./atividade4 1
Thread : 1 esta executando...
Extra : esta executando...
soma = 10 
soma = 20 
soma = 30 
soma = 40 
soma = 50 
soma = 60 
soma = 70 
soma = 80 
soma = 90 
soma = 100 
soma = 110 
soma = 120 
soma = 130 
soma = 140 
soma = 150 
soma = 160 
soma = 170 
soma = 180 
soma = 190 
soma = 200 
Extra : terminou!
Thread : 1 terminou!
Valor de 'soma' = 100000
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ ./atividade4 2
Thread : 1 esta executando...
Extra : esta executando...
soma = 10 
soma = 20 
soma = 30 
soma = 40 
soma = 50 
soma = 60 
soma = 70 
soma = 80 
soma = 90 
soma = 100 
soma = 110 
soma = 120 
soma = 130 
soma = 140 
soma = 150 
soma = 160 
soma = 170 
soma = 180 
soma = 190 
soma = 200 
Extra : terminou!
Thread : 1 terminou!
Valor de 'soma' = 100000
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ ./atividade4 4
Thread : 1 esta executando...
Extra : esta executando...
soma = 10 
soma = 20 
soma = 30 
soma = 40 
soma = 50 
soma = 60 
soma = 70 
soma = 80 
soma = 90 
soma = 100 
soma = 110 
soma = 120 
soma = 130 
soma = 140 
soma = 150 
soma = 160 
soma = 170 
soma = 180 
soma = 190 
soma = 200 
Extra : terminou!
Thread : 1 terminou!
Valor de 'soma' = 100000
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ ./atividade4 8
Thread : 1 esta executando...
Extra : esta executando...
soma = 10 
soma = 20 
soma = 30 
soma = 40 
soma = 50 
soma = 60 
soma = 70 
soma = 80 
soma = 90 
soma = 100 
soma = 110 
soma = 120 
soma = 130 
soma = 140 
soma = 150 
soma = 160 
soma = 170 
soma = 180 
soma = 190 
soma = 200 
Extra : terminou!
Thread : 1 terminou!
Valor de 'soma' = 100000
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab5$ 