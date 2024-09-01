# Instruções

gcc -o gerar_dados gerar_dados.c
gcc -o lab2_atividade3 lab2_atividade3.c -pthread

./gerar_dados <tamanho do vetor>
./lab2_atividade3 <threads> dados.bin

# Testes

## Testes com Double

### Vetor Pequeno:

joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./gerar_dados 10
joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./lab2_atividade3 2 dados.bin
Produto interno sequencial: 27.259979

Produto interno concorrente: 27.259979
Variação relativa: 0.000000

### Vetor Médio:

joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./gerar_dados 100
joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./lab2_atividade3 5 dados.bin
Produto interno sequencial: 27.259979
Produto interno concorrente: 27.259979
Variação relativa: 0.000000

### Vetor Grande:

joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./gerar_dados 10000
joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./lab2_atividade3 20 dados.bin
Produto interno sequencial: 27.259979
Produto interno concorrente: 27.259979
Variação relativa: 0.000000

### Vetor e Threads Ímpares

joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./gerar_dados 11
joao@DESKTOP-ERIJR87:~/workspace/computacao_concorrente/lab2$ ./lab2_atividade3 3 dados.bin
Produto interno sequencial: 27.259979
Produto interno concorrente: 27.259979
Variação relativa: 0.000000

## Teste com Float

### Vetor Grande:

joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab2$ ./gerar_dados 10000
joao@Alvarenga-Laptop:~/workspace/computacao_concorrente/lab2$ ./lab2_atividade3 20 dados.bin
Produto interno sequencial: -149278534682876294320251769465077760.000000
Produto interno concorrente: 27.259981
Variação relativa: 1.000000

# Conclusão

Podemos ver que não há grandes diferenças entre os resultados, apenas quando mudamos de double para float, isso acontece devido à menor precisão, float é mais suscetível a erros de arredondamento do que double, o que pode levar a um acumulaço de perdas de precisão em operações aritméticas, gerando uma diferença maior