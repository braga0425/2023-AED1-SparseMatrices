#include <stdio.h>
#include <stdlib.h>

// Estrutura da célula da matriz
struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
};
typedef struct matrix Matrix;

// Função para criar uma matriz esparsa
Matrix* matrix_create() {
    int m, n;
    scanf("%d %d", &m, &n);
    
    Matrix* head = (Matrix*)malloc(sizeof(Matrix));
    head->line = -1;
    head->column = -1;
    head->info = 0;
    head->right = head;
    head->below = head;
    
    Matrix* current_row = head;
    Matrix* previous_in_column[n];
    
    for (int j = 0; j < n; j++) {
        previous_in_column[j] = head;
    }
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float value;
            scanf("%f", &value);
            if (value != 0) {
                Matrix* new_cell = (Matrix*)malloc(sizeof(Matrix));
                new_cell->line = i;
                new_cell->column = j;
                new_cell->info = value;
                
                // Conectar na linha
                current_row->right = new_cell;
                new_cell->right = head;
                current_row = new_cell;
                
                // Conectar na coluna
                new_cell->below = previous_in_column[j]->below;
                previous_in_column[j]->below = new_cell;
                previous_in_column[j] = new_cell;
            }
        }
    }
    
    return head;
}

// Função para destruir a matriz e liberar memória
void matrix_destroy(Matrix* m) {
    Matrix* current = m->right;
    while (current != m) {
        Matrix* temp = current;
        current = current->right;
        free(temp);
    }
    free(m);
}

// Função para imprimir a matriz
void matrix_print(Matrix* m) {
    Matrix* current_row = m->below;
    while (current_row != m) {
        Matrix* current_cell = current_row->right;
        while (current_cell != current_row) {
            printf("%d %d %.1f\n", current_cell->line, current_cell->column, current_cell->info);
            current_cell = current_cell->right;
        }
        current_row = current_row->below;
    }
}

// Função para somar duas matrizes
Matrix* matrix_add(Matrix* m, Matrix* n) {
    // Verificar se as matrizes têm o mesmo tamanho
    if (m->line != n->line || m->column != n->column) {
        fprintf(stderr, "Erro: Matrizes de tamanhos diferentes.\n");
        return NULL;
    }

    int numRows = m->line;
    int numCols = m->column;

    // Criar uma matriz resultante com o mesmo tamanho
    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->line = numRows;
    result->column = numCols;
    result->info = 0;
    result->right = result;
    result->below = result;

    // Arrays para rastrear a cabeça de linha e coluna na matriz resultante
    Matrix* headRows[numRows];
    Matrix* headCols[numCols];

    // Inicializar cabeças de linha
    for (int i = 0; i < numRows; i++) {
        headRows[i] = result;
    }

    // Inicializar cabeças de coluna
    for (int j = 0; j < numCols; j++) {
        headCols[j] = result;
    }

    // Percorrer a matriz m e adicionar elementos à matriz resultante
    Matrix* current_m = m->below;
    while (current_m != m) {
        Matrix* current_cell_m = current_m->right;
        while (current_cell_m != current_m) {
            int i = current_cell_m->line;
            int j = current_cell_m->column;
            float value_m = current_cell_m->info;

            // Adicionar o elemento de m à matriz resultante
            Matrix* new_cell = (Matrix*)malloc(sizeof(Matrix));
            new_cell->line = i;
            new_cell->column = j;
            new_cell->info = value_m;

            // Conectar na linha
            new_cell->right = result;
            new_cell->below = headRows[i]->below;
            headRows[i]->below = new_cell;
            headRows[i] = new_cell;

            // Conectar na coluna
            new_cell->below = headCols[j]->below;
            headCols[j]->below = new_cell;
            headCols[j] = new_cell;

            current_cell_m = current_cell_m->right;
        }
        current_m = current_m->below;
    }

    return result;
}

// Função para multiplicar duas matrizes
Matrix* matrix_multiply(Matrix* m, Matrix* n) {
    // Verificar se as matrizes podem ser multiplicadas
    if (m->column != n->line) {
        fprintf(stderr, "Erro: Matrizes não podem ser multiplicadas.\n");
        return NULL;
    }

    int numRows_m = m->line;
    int numCols_m = m->column;
    int numRows_n = n->line;
    int numCols_n = n->column;

    // Criar uma matriz resultante com o tamanho apropriado
    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->line = numRows_m;
    result->column = numCols_n;
    result->info = 0;
    result->right = result;
    result->below = result;

    // Arrays para rastrear a cabeça de linha e coluna na matriz resultante
    Matrix* headRows[numRows_m];
    Matrix* headCols[numCols_n];

    // Inicializar cabeças de linha
    for (int i = 0; i < numRows_m; i++) {
        headRows[i] = result;
    }

    // Inicializar cabeças de coluna
    for (int j = 0; j < numCols_n; j++) {
        headCols[j] = result;
    }

    // Realizar a multiplicação
    for (int i = 0; i < numRows_m; i++) {
        for (int j = 0; j < numCols_n; j++) {
            float dot_product = 0.0;
            for (int k = 0; k < numCols_m; k++) {
                // Encontrar elementos relevantes nas matrizes m e n
                Matrix* current_m = m->below;
                while (current_m->line != i) {
                    current_m = current_m->below;
                }
                Matrix* current_n = n->right;
                while (current_n->column != j) {
                    current_n = current_n->right;
                }

                // Multiplicar e somar ao produto escalar
                dot_product += current_m->info * current_n->info;
            }

            // Se o produto escalar for diferente de zero, adicionar à matriz resultante
            if (dot_product != 0) {
                // Adicionar o elemento resultante à matriz resultante
                Matrix* new_cell = (Matrix*)malloc(sizeof(Matrix));
                new_cell->line = i;
                new_cell->column = j;
                new_cell->info = dot_product;

                // Conectar na linha
                new_cell->right = result;
                new_cell->below = headRows[i]->below;
                headRows[i]->below = new_cell;
                headRows[i] = new_cell;

                // Conectar na coluna
                new_cell->below = headCols[j]->below;
                headCols[j]->below = new_cell;
                headCols[j] = new_cell;
            }
        }
    }

    return result;
}

// Função para calcular a transposta de uma matriz
Matrix* matrix_transpose(Matrix* m) {
    int numRows = m->line;
    int numCols = m->column;

    // Criar uma matriz resultante com linhas e colunas invertidas
    Matrix* result = (Matrix*)malloc(sizeof(Matrix));
    result->line = numCols;
    result->column = numRows;
    result->info = 0;
    result->right = result;
    result->below = result;

    // Arrays para rastrear a cabeça de linha e coluna na matriz resultante
    Matrix* headRows[numRows];
    Matrix* headCols[numCols];

    // Inicializar cabeças de linha
    for (int i = 0; i < numRows; i++) {
        headRows[i] = result;
    }

    // Inicializar cabeças de coluna
    for (int j = 0; j < numCols; j++) {
        headCols[j] = result;
    }

    // Percorrer a matriz original e criar a transposta
    Matrix* current_m = m->below;
    while (current_m != m) {
        Matrix* current_cell_m = current_m->right;
        while (current_cell_m != current_m) {
            int i = current_cell_m->line;
            int j = current_cell_m->column;
            float value_m = current_cell_m->info;

            // Adicionar o elemento da matriz original à transposta
            Matrix* new_cell = (Matrix*)malloc(sizeof(Matrix));
            new_cell->line = j; // Trocar linhas e colunas
            new_cell->column = i;
            new_cell->info = value_m;

            // Conectar na linha
            new_cell->right = result;
            new_cell->below = headRows[j]->below;
            headRows[j]->below = new_cell;
            headRows[j] = new_cell;

            // Conectar na coluna
            new_cell->below = headCols[i]->below;
            headCols[i]->below = new_cell;
            headCols[i] = new_cell;

            current_cell_m = current_cell_m->right;
        }
        current_m = current_m->below;
    }

    return result;
}

// Função para obter o valor de um elemento na matriz
float matrix_getelem(Matrix* m, int x, int y) {
    // Percorrer a matriz para encontrar o elemento desejado
    Matrix* current = m->below;
    while (current != m) {
        Matrix* current_cell = current->right;
        while (current_cell != current) {
            if (current_cell->line == x && current_cell->column == y) {
                return current_cell->info; // Elemento encontrado
            }
            current_cell = current_cell->right;
        }
        current = current->below;
    }
    return 0.0; // Elemento não encontrado
}

// Função para definir o valor de um elemento na matriz
void matrix_setelem(Matrix* m, int x, int y, float elem) {
    // Percorrer a matriz para encontrar o elemento desejado
    Matrix* current = m->below;
    while (current != m) {
        Matrix* current_cell = current->right;
        while (current_cell != current) {
            if (current_cell->line == x && current_cell->column == y) {
                current_cell->info = elem; // Atualizar o valor do elemento
                return;
            }
            current_cell = current_cell->right;
        }
        current = current->below;
    }
    // Se o elemento não existir, criar um novo elemento
    Matrix* new_cell = (Matrix*)malloc(sizeof(Matrix));
    new_cell->line = x;
    new_cell->column = y;
    new_cell->info = elem;

    // Encontrar a posição correta para inserir o novo elemento na linha
    Matrix* prev = m;
    Matrix* current_row = m->below;
    while (current_row != m && current_row->line < x) {
        prev = current_row;
        current_row = current_row->below;
    }

    // Conectar o novo elemento na linha
    prev->below = new_cell;
    new_cell->below = current_row;

    // Encontrar a posição correta para inserir o novo elemento na coluna
    prev = m;
    Matrix* current_col = m->right;
    while (current_col != m && current_col->column < y) {
        prev = current_col;
        current_col = current_col->right;
    }

    // Conectar o novo elemento na coluna
    prev->right = new_cell;
    new_cell->right = current_col;
}


int main() {
    Matrix* A = matrix_create();
    matrix_print(A);
    matrix_destroy(A);
    return 0;
}
