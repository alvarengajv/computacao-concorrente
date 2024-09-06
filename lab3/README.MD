# Laboratório: 3
Disciplina: Programacao Concorrente 

Profa.: Silvana Rossetto 

Codigo: Multiplicação de Matrizes

João Vitor Lopes Alvarenga - 120152276


# Instruções

## Acessar pasta
cd lab3

## Gerador de matrizes

### Compliação gerador de matrizes

gcc -o gerador_matrizes gerador_matrizes.c

### Execução gerador de matrizes

./gerador_matrizes 500 500 matriz1.bin matriz2.bin

./gerador_matrizes 1000 1000 matriz1.bin matriz2.bin

./gerador_matrizes 2000 2000 matriz1.bin matriz2.bin

## Multiplicador de matrizes

### Compilação multiplicador de matrizes sequencial

gcc -o multiplicador_matrizes_seq multiplicador_matrizes_seq.c -lm

### Execução multiplicador de matrizes sequencial

./multiplicador_matrizes_seq matriz1.bin matriz2.bin resultado_seq.bin

### Compilação multiplicador de matrizes concorrente

gcc -o multiplicador_matrizes_conc multiplicador_matrizes_conc.c -lm

### Execução multiplicador de matrizes concorrente

./multiplicador_matrizes_conc matriz1.bin matriz2.bin resultado_conc.bin 1

./multiplicador_matrizes_conc matriz1.bin matriz2.bin resultado_conc.bin 2

./multiplicador_matrizes_conc matriz1.bin matriz2.bin resultado_conc.bin 4

./multiplicador_matrizes_conc matriz1.bin matriz2.bin resultado_conc.bin 8
