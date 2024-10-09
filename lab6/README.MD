# Laboratório: 6
Disciplina: Programacao Concorrente 

Profa.: Silvana Rossetto 

Codigo: Leitores e escritores com prioridade para escrita.

Aluno: João Vitor Lopes Alvarenga - 120152276

# Instruções

## Acessar pasta
cd lab/lab6

## Compilar código
gcc -o atividade3 atividade3.c list_int.c -lpthread

## Executar código
./atividade3 <numero de threads>

# Análise:
O programa assegura que escritores tenham prioridade sobre os leitores. Isso é visível nos logs:
Quando uma thread de escrita solicita acesso ao recurso, todas as novas operações de leitura são bloqueadas até que a escrita seja concluída. Esse comportamento pode ser observado nos logs de bloqueio, como Leitura bloqueada por operação de escrita. Após a conclusão da escrita, as leituras são retomadas, como indicado pela mensagem Leitura permitida.Esse comportamento garante que a consistência dos dados seja mantida, já que não ocorrem leituras durante a modificação da lista.

# Demonstração de prioridade de escrita:
O teste foi feito com 100 operações para facilitar a demonstração

joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab/lab6$ gcc -o atividade3 atividade3.c list_int.c -lpthread
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab/lab6$ ./atividade3 4
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 2: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 1: Leitura permitida.
Thread 2: Leitura permitida.
Thread 2: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 1: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 2: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Iniciando escrita...
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 2: Leitura bloqueada por operação de escrita.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 0: Leitura permitida.
Thread 3: Escrita concluída.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Iniciando remoção...
Thread 3: Remoção concluída.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 3: Leitura permitida.
Thread 0: in=0 out=0 read=25
Thread 1: in=0 out=0 read=25
Thread 2: in=0 out=0 read=4
Thread 3: in=1 out=1 read=23
Tempo: 0.002908