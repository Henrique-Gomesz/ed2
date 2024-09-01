#include <stdio.h>
#include <stdlib.h>

struct lista {
    struct lista *prox,*ant;
    int info;
}; typedef struct lista Lista;

struct descritor {
    int quantidade;
    struct lista *inicio,*fim;
};typedef struct  descritor Descritor;

void init(Descritor **desc) {
    *desc = (Descritor *)malloc(sizeof(Descritor *));
    (*desc)->inicio =(*desc)->fim = NULL;
    (*desc)->quantidade = 0;
}

void pushInit(Descritor *desc, int info) {
    Lista *novo = (struct lista *)malloc(sizeof(struct lista));
    novo->info = info;
    novo->prox = desc->inicio;
    novo->ant = NULL;

    if(desc->inicio == NULL) {
        desc->fim = desc->inicio = novo;
        desc->quantidade++;
    }
    else {
        desc->inicio->ant = novo;
        desc->inicio = novo;
    }

}

int main(void)
{
    printf("Hello, World!\n");
    return 0;
}
