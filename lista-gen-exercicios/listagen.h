//
// Created by gomes on 29/09/2024.
//

#ifndef LIST_GEN_H
#define LIST_GEN_H
typedef struct listagen ListaGen;

struct reg_lista {
    ListaGen *cabeca;
    ListaGen *cauda;
};

struct info_lista {
    char info[8];
    struct reg_lista lista;
};

struct listagen {
    char terminal;
    union {
        struct info_lista no;
    };
};

ListaGen * Criat(char *info) {
    ListaGen *l = (ListaGen *)malloc(sizeof(ListaGen));
    l->terminal = 1;
    strcpy((l->no).info,info);
    return l;
}

char Nula(ListaGen *l) {
    return  l == NULL;
}

char Atomo(ListaGen *l) {
    return !Nula(l) && l->terminal;
}

ListaGen * head(ListaGen *l) {
    if(Nula(l) || Atomo(l))
        return NULL;
    else
        return l->no.lista.cabeca;
}

ListaGen *Tail(ListaGen *L)
{
    if (Nula(L) || Atomo(L))
    {
        printf("Tail: argumento deve ser lista não vazia!");
        return NULL;
    }
    else
        return L->no.lista.cauda;
}

void exibe(ListaGen *l) {
    if(Atomo(l))
        printf("%s",l->no.info);
    else {
        printf("[");
        while (!Nula(l)) {
            exibe(head(l));
            l = Tail(l);
            if(!Nula(l))
                printf(",");
        }
        printf("]");
    }
}

ListaGen *Cons(ListaGen *H, ListaGen *T)
{
    if (Atomo(T))
    {
        printf("Cons: Segundo arqumento nao pode ser Atomo!");
        return NULL;
    }
    else
    {
        ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
        L->terminal = 0;
        L->no.lista.cabeca = H;
        L->no.lista.cauda = T;
        return L;
    }
}

void exibeAtomo(ListaGen *l) {
    if(!Nula(l)) {
        if(Atomo(l))
            printf("%s",l->no.info);
        else {
            exibeAtomo(head(l));
            exibeAtomo(Tail(l));
        }
    }
}

void exibeAtomoRecursive(ListaGen *l) {
    if(!Nula(l)) {
        if(Atomo(l))
            printf("%s",l->no.info);
        else {
            exibeAtomoRecursive(head(l));
            exibeAtomoRecursive(Tail(l));
        }
    }
}

void destruct(ListaGen **l) {
    if(!Nula(*l)) {
        if(Atomo(*l))
            free(*l);
        else {
            destruct(head(*l));
            destruct(Tail(*l));
            free(*l);
        }
        *l = NULL;
    }
}
#endif //LIST_GEN_H
