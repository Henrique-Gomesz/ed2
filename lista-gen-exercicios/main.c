#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listagen.h"
#include "pilha.h"


void selectionSort(ListaGen *l) {
    ListaGen  *menor,*aux1 = l, *aux2 = l;
    char auxInfo[8];

    while(aux1 != NULL) {
       if(Atomo(head(aux1))) {
           aux2 = aux1;
           menor = head(aux1);
           while(aux2 != NULL) {
               if(Atomo(head(aux2))) {
                   if(strcmp(head(aux2)->no.info,menor->no.info) < 0) {
                       menor = head(aux2);
                   }
               }
               aux2 = Tail(aux2);
           }
           strcpy(auxInfo,aux1->no.info);
           strcpy(aux1->no.info,menor->no.info);
           strcpy(menor->no.info,auxInfo);
       }
        aux1 = Tail(aux1);
    }
}

void ordenaListaGen(ListaGen *l) {
    Fila *fila = criar_fila();
    Fila *fila1 = criar_fila();

    enqueue(fila,l);
    enqueue(fila1,l);
    while(!isEmpty(fila)) {
        l = dequeue(fila);
        while(!Nula(l)) {
            if(!Nula(head(l)) && !Atomo(head(l))) {
                enqueue(fila,head(l));
                enqueue(fila1,head(l));
            }
            l = Tail(l);
        }
    }
    while(!isEmpty(fila1)) {
        l = dequeue(fila1);
        selectionSort(l);
    }
}


int main(void) {
    ListaGen *l = (ListaGen*)malloc(sizeof(ListaGen));
    l = Cons(Cons(Criat("c"),Cons(Criat("b"),NULL)),Cons(Criat("a"),NULL));
    ordenaListaGen(l);
    exibe(l);
    printf("\n");
    exibeAtomoRecursive(l);
    free(l); // Always free allocated memory
    return 0;
}
