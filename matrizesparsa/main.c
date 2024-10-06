#include <stdio.h>
#include <stdlib.h>

#define nl 5 // Número de linhas
#define nc 6 // Número de colunas

// Estrutura de Dados para Lista Cruzada
struct matrizEsp {
    int lin, col, valor;
    struct matrizEsp *pc, *pl; // Ponteiros para a próxima coluna (pc) e próxima linha (pl)
};

typedef struct matrizEsp MatEsp;

// Função para inicializar a matriz esparsa
void inicializar(MatEsp *vetlin[], MatEsp *vetcol[]) {
    for (int i = 0; i < nl; i++) {
        vetlin[i] = NULL; // Inicializa cada linha com NULL
    }
    for (int j = 0; j < nc; j++) {
        vetcol[j] = NULL; // Inicializa cada coluna com NULL
    }
}

// Função para verificar se uma posição já está ocupada
void verificaOcupado(MatEsp *vetlin[], int lin, int col, MatEsp **aux) {
    *aux = vetlin[lin];
    while (*aux != NULL && (*aux)->col != col) {
        *aux = (*aux)->pl;
    }
}

// Função para inserir um elemento na matriz esparsa
void insere(MatEsp *vetlin[], MatEsp *vetcol[], int lin, int col, int valor) {
    MatEsp *ant, *aux, *nova;

    if (lin >= 0 && lin < nl && col >= 0 && col < nc) {
        verificaOcupado(vetlin, lin, col, &aux);
        if (aux != NULL) {
            aux->valor = valor; // Atualiza o valor se a posição já existe
        } else {
            nova = (MatEsp*) malloc(sizeof(MatEsp)); // Cria um novo nó
            nova->lin = lin;
            nova->col = col;
            nova->valor = valor;
            nova->pl = NULL;
            nova->pc = NULL;

            // Ligação horizontal (inserção nas linhas)
            if (vetlin[lin] == NULL || col < vetlin[lin]->col) {
                nova->pl = vetlin[lin];
                vetlin[lin] = nova;
            } else {
                aux = vetlin[lin];
                while (aux->pl != NULL && aux->pl->col < col) {
                    aux = aux->pl;
                }
                nova->pl = aux->pl;
                aux->pl = nova;
            }

            // Ligação vertical (inserção nas colunas)
            if (vetcol[col] == NULL || lin < vetcol[col]->lin) {
                nova->pc = vetcol[col];
                vetcol[col] = nova;
            } else {
                aux = vetcol[col];
                while (aux->pc != NULL && aux->pc->lin < lin) {
                    aux = aux->pc;
                }
                nova->pc = aux->pc;
                aux->pc = nova;
            }
        }
    } else {
        printf("As coordenadas estão fora da Matriz!\n");
    }
}

// Função para exibir a matriz esparsa
void exibir(MatEsp *vetlin[]) {
    for (int i = 0; i < nl; i++) {
        MatEsp *aux = vetlin[i];
        for (int j = 0; j < nc; j++) {
            if (aux != NULL && aux->col == j) {
                printf("%4d ", aux->valor);
                aux = aux->pl;
            } else {
                printf("%4d ", 0); // Elemento nulo
            }
        }
        printf("\n");
    }
}

// Função para excluir um elemento da matriz esparsa
void excluir(MatEsp *vetlin[], MatEsp *vetcol[], int lin, int col) {
    MatEsp *aux, *ant;

    // Excluir da lista de linhas
    aux = vetlin[lin];
    ant = NULL;
    while (aux != NULL && aux->col != col) {
        ant = aux;
        aux = aux->pl;
    }
    if (aux != NULL) {
        if (ant == NULL) {
            vetlin[lin] = aux->pl;
        } else {
            ant->pl = aux->pl;
        }

        // Excluir da lista de colunas
        aux = vetcol[col];
        ant = NULL;
        while (aux != NULL && aux->lin != lin) {
            ant = aux;
            aux = aux->pc;
        }
        if (aux != NULL) {
            if (ant == NULL) {
                vetcol[col] = aux->pc;
            } else {
                ant->pc = aux->pc;
            }
        }

        free(aux);
    }
}

// Função para liberar a memória da matriz esparsa
void liberar(MatEsp *vetlin[], MatEsp *vetcol[]) {
    for (int i = 0; i < nl; i++) {
        MatEsp *aux = vetlin[i];
        while (aux != NULL) {
            MatEsp *temp = aux;
            aux = aux->pl;
            free(temp);
        }
    }
}

int main() {
    MatEsp *vetlin[nl]; // Vetor de linhas
    MatEsp *vetcol[nc]; // Vetor de colunas

    inicializar(vetlin, vetcol);

    // Inserir elementos na matriz esparsa
    insere(vetlin, vetcol, 0, 4, 9);
    insere(vetlin, vetcol, 1, 1, -3);
    insere(vetlin, vetcol, 2, 4, 4);
    insere(vetlin, vetcol, 3, 0, 5);
    insere(vetlin, vetcol, 3, 2, 1);

    // Exibir a matriz esparsa
    printf("Matriz Esparsa:\n");
    exibir(vetlin);

    // Excluir um elemento e exibir novamente
    excluir(vetlin, vetcol, 3, 2);
    printf("\nMatriz após exclusão do elemento (3,2):\n");
    exibir(vetlin);

    // Liberar a memória
    liberar(vetlin, vetcol);

    return 0;
}
