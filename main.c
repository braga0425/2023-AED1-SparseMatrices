#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main() {
    Matrix* matrix1 = matrix_create();
    Matrix* matrix2 = matrix_create();

    // Leitura da primeira matriz a partir de "matrix1.txt"
    FILE* file1 = fopen("matrix1.txt", "r");
    if (file1 == NULL) {
        perror("Erro na abertura do arquivo");
        exit(EXIT_FAILURE);
    }

    int numRows1, numCols1;
    fscanf(file1, "%d %d", &numRows1, &numCols1);

    int x1, y1;
    float elem1;
    while (fscanf(file1, "%d %d %f", &x1, &y1, &elem1) == 3) {
        matrix_setelem(matrix1, x1, y1, elem1);
    }
    fclose(file1);

    // Imprimir a primeira matriz
    printf("Matriz 1:\n");
    matrix_print(matrix1);
    printf("\n");

    // Leitura da segunda matriz a partir de "matrix2.txt"
    FILE* file2 = fopen("matrix2.txt", "r");
    if (file2 == NULL) {
        perror("Erro na abertura do arquivo");
        exit(EXIT_FAILURE);
    }

    int numRows2, numCols2;
    fscanf(file2, "%d %d", &numRows2, &numCols2);

    int x2, y2;
    float elem2;
    while (fscanf(file2, "%d %d %f", &x2, &y2, &elem2) == 3) {
        matrix_setelem(matrix2, x2, y2, elem2);
    }
    fclose(file2);

    // Imprimir a segunda matriz
    printf("Matriz 2:\n");
    matrix_print(matrix2);
    printf("\n");

    // Realizar a soma das duas matrizes
    Matrix* sum_result = matrix_add(matrix1, matrix2);

    // Imprimir o resultado da soma
    printf("Soma das Matrizes:\n");
    matrix_print(sum_result);
    printf("\n");

    // Realizar a multiplicação das duas matrizes
    Matrix* multiply_result = matrix_multiply(matrix1, matrix2);

    // Imprimir o resultado da multiplicação
    printf("Multiplicacao das Matrizes:\n");
    matrix_print(multiply_result);
    printf("\n");

    // Calcular e imprimir a matriz transposta de multiply_result
    Matrix* transpose_result = matrix_transpose(matrix1);
    printf("Matriz Transposta:\n");
    matrix_print(transpose_result);
    printf("\n");

    // Destruir as matrizes quando não forem mais necessárias
    matrix_destroy(matrix1);
    matrix_destroy(matrix2);
    matrix_destroy(sum_result);
    matrix_destroy(multiply_result);
    matrix_destroy(multiply_result);
    matrix_destroy(transpose_result); // Não se esqueça de destruir a matriz transposta

    return 0;
}
