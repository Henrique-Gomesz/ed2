#include <stdio.h>
#include <stdlib.h>
#include "listagen.h"

typedef struct No {
    ListaGen* dado;
    struct No* proximo;
} No;

typedef struct {
    No* frente;
    No* tras;
} Fila;

Fila* criar_fila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    fila->frente = fila->tras = NULL;
    return fila;
}

int isEmpty(Fila* fila) {
    return fila->frente == NULL;
}

void enqueue(Fila* fila, ListaGen* valor) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->dado = valor;
    novo_no->proximo = NULL;

    if (fila->tras == NULL) {
        fila->frente = fila->tras = novo_no;
        return;
    }

    fila->tras->proximo = novo_no;
    fila->tras = novo_no;
}

ListaGen* dequeue(Fila* fila) {
    if (isEmpty(fila)) {
        printf("Fila está vazia! Não é possível desenfileirar.\n");
        return NULL;
    }

    No* temp = fila->frente;
    ListaGen* dado = temp->dado;
    fila->frente = fila->frente->proximo;

    // Se a fila ficar vazia após a remoção
    if (fila->frente == NULL) {
        fila->tras = NULL;
    }

    free(temp);
    return dado;
}

ListaGen* firstPosition(Fila* fila) {
    if (isEmpty(fila)) {
        printf("Fila está vazia!\n");
        return NULL;
    }
    return fila->frente->dado;
}


void destructQueue(Fila* fila) {
    while (!isEmpty(fila)) {
        destruct(fila);
    }
    free(fila);
}