#include <stdio.h>
#include <stdlib.h>

struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
};

typedef struct matrix Matrix;

Matrix* matrix_create(void) {
    Matrix* head = (Matrix*)malloc(sizeof(Matrix));
    if (head == NULL) {
        perror("Erro na alocação de memória");
        exit(EXIT_FAILURE);
    }
    
    head->right = head;
    head->below = head;
    head->line = -1; // Cabeça da linha
    head->column = -1; // Cabeça da coluna
    head->info = 0.0; // Valor padrão para a célula cabeça
    
    return head;
}

float matrix_getelem(Matrix* m, int x, int y) {
    Matrix* row_head = m->below;
    while (row_head != m) {
        if (row_head->line == x) {
            Matrix* current = row_head->right;
            while (current != row_head) {
                if (current->column == y) {
                    return current->info;
                }
                current = current->right;
            }
            break;
        }
        row_head = row_head->below;
    }
    return 0.0; // Valor padrão para elementos não encontrados
}

void matrix_setelem(Matrix* m, int x, int y, float elem) {
    Matrix* row_head = m->below;
    Matrix* prev_row = m;
    while (row_head != m && row_head->line < x) {
        prev_row = row_head;
        row_head = row_head->below;
    }
    
    if (row_head->line != x) {
        // A linha x ainda não existe, precisamos criá-la
        Matrix* new_row = (Matrix*)malloc(sizeof(Matrix));
        if (new_row == NULL) {
            perror("Erro na alocação de memória");
            exit(EXIT_FAILURE);
        }
        new_row->line = x;
        new_row->column = -1; // Cabeça da coluna
        new_row->info = 0.0;
        new_row->right = new_row;
        
        // Inserir nova linha
        prev_row->below = new_row;
        new_row->below = row_head;
        row_head = new_row;
    }
    
    Matrix* current = row_head->right;
    Matrix* prev = row_head;
    
    while (current != row_head && current->column < y) {
        prev = current;
        current = current->right;
    }
    
    if (current->column == y) {
        // O elemento (x, y) já existe, atualize-o
        current->info = elem;
    } else {
        // O elemento (x, y) não existe, crie-o
        Matrix* new_elem = (Matrix*)malloc(sizeof(Matrix));
        if (new_elem == NULL) {
            perror("Erro na alocação de memória");
            exit(EXIT_FAILURE);
        }
        new_elem->line = x;
        new_elem->column = y;
        new_elem->info = elem;
        
        // Inserir novo elemento
        prev->right = new_elem;
        new_elem->right = current;
    }
}

void matrix_print(Matrix* m) {
    int maxLine = -1, maxColumn = -1;

    // Encontre o número máximo de linhas e colunas com base nos elementos existentes
    Matrix* row_head = m->below;
    while (row_head != m) {
        Matrix* current = row_head->right;
        while (current != row_head) {
            if (current->line > maxLine) {
                maxLine = current->line;
            }
            if (current->column > maxColumn) {
                maxColumn = current->column;
            }
            current = current->right;
        }
        row_head = row_head->below;
    }

    // Imprima a matriz preenchendo com zero onde não há valores
    for (int i = 1; i <= maxLine; i++) {
        for (int j = 1; j <= maxColumn; j++) {
            float elem = matrix_getelem(m, i, j);
            printf("%.2f\t", elem);
        }
        printf("\n");
    }
}

Matrix* matrix_add(Matrix* m, Matrix* n) {
    // Verificar se as matrizes têm o mesmo número de linhas e colunas
    if (matrix_getelem(m, 1, 1) == 0.0 || matrix_getelem(n, 1, 1) == 0.0) {
        printf("As matrizes têm tamanhos diferentes.\n");
        return NULL;
    }

    // Criar uma matriz resultante com o mesmo tamanho que m e n
    Matrix* result = matrix_create();

    // Obter o número máximo de linhas e colunas das matrizes m e n
    int maxLine = -1, maxColumn = -1;
    Matrix* row_head_m = m->below;
    Matrix* row_head_n = n->below;

    while (row_head_m != m || row_head_n != n) {
        int line_m = row_head_m->line;
        int line_n = row_head_n->line;

        if (line_m > maxLine) {
            maxLine = line_m;
        }
        if (line_n > maxLine) {
            maxLine = line_n;
        }

        // Percorra ambas as matrizes para encontrar o número máximo de colunas
        Matrix* current_m = row_head_m->right;
        Matrix* current_n = row_head_n->right;

        while (current_m != row_head_m || current_n != row_head_n) {
            int column_m = current_m->column;
            int column_n = current_n->column;

            if (column_m > maxColumn) {
                maxColumn = column_m;
            }
            if (column_n > maxColumn) {
                maxColumn = column_n;
            }

            current_m = current_m->right;
            current_n = current_n->right;
        }

        row_head_m = row_head_m->below;
        row_head_n = row_head_n->below;
    }

    // Preencher a matriz resultante com a soma dos elementos correspondentes de m e n
    for (int i = 1; i <= maxLine; i++) {
        for (int j = 1; j <= maxColumn; j++) {
            float elem_m = matrix_getelem(m, i, j);
            float elem_n = matrix_getelem(n, i, j);
            matrix_setelem(result, i, j, elem_m + elem_n);
        }
    }

    return result;
}

void matrix_destroy(Matrix* m) {
    Matrix* row_head = m->below;
    while (row_head != m) {
        Matrix* current = row_head->right;
        while (current != row_head) {
            Matrix* temp = current;
            current = current->right;
            free(temp); // Liberar cada célula da matriz
        }
        Matrix* temp = row_head;
        row_head = row_head->below;
        free(temp); // Liberar a cabeça da linha
    }
    free(m); // Liberar a cabeça da matriz
}

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
    Matrix* result = matrix_add(matrix1, matrix2);

    // Imprimir o resultado da soma
    printf("Soma das matrizes:\n");
    matrix_print(result);

    // Destruir as matrizes quando não forem mais necessárias
    matrix_destroy(matrix1);
    matrix_destroy(matrix2);
    matrix_destroy(result);

    return 0;
}
