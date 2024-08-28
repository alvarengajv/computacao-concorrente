#include <stdio.h>
#include <stdlib.h>

int main() {
    int N = 100; // tamanho dos vetores
    float vetor_a[N], vetor_b[N];
    double produto_interno = 0.0;

    // Preenchendo os vetores com valores aleatórios
    for (int i = 0; i < N; i++) {
        vetor_a[i] = (float)rand() / RAND_MAX;
        vetor_b[i] = (float)rand() / RAND_MAX;
        produto_interno += vetor_a[i] * vetor_b[i];
    }

    // Escrevendo os vetores e o resultado em um arquivo binário
    FILE *arquivo = fopen("dados.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo\n");
        return 1;
    }

    fwrite(&N, sizeof(int), 1, arquivo);
    fwrite(vetor_a, sizeof(float), N, arquivo);
    fwrite(vetor_b, sizeof(float), N, arquivo);
    fwrite(&produto_interno, sizeof(double), 1, arquivo);

    fclose(arquivo);
    return 0;
}
