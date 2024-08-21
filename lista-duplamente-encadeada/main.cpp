#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 1024
#define MAX_TEXTO 256

typedef struct autor {
    struct autor *prox;
    char nome[MAX_TEXTO];
    char sobrenome[MAX_TEXTO];
} Autor;

typedef struct listaAutor {
    Autor *autor;
    struct listaAutor *prox;
} ListaAutor;

typedef struct livros {
    struct livros *ant;
    struct livros *prox;
    ListaAutor *lista_autores;
    char titulo[100];
    int ano;
    int paginas;
} Livros;

typedef struct editora {
    Livros *livros;
    char nome[100];
} Editora;

typedef struct {
    char autores[MAX_TEXTO];
    char titulo_livro[MAX_TEXTO];
    char editora[MAX_TEXTO];
    int ano;
    int paginas;
} Livro;

void importarLivros(const char *arquivoTexto, const char *arquivoBinario) {
    FILE *arquivoTxt = fopen(arquivoTexto, "r");
    if (arquivoTxt == NULL) {
        perror("Erro ao abrir o arquivo de texto");
        return;
    }

    FILE *arquivoBin = fopen(arquivoBinario, "wb");
    if (arquivoBin == NULL) {
        perror("Erro ao abrir o arquivo binário");
        fclose(arquivoTxt);
        return;
    }

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivoTxt)) {
        Livro livro;
        char *token = strtok(linha, "|");

        strncpy(livro.autores, token ? token : "", MAX_TEXTO);

        token = strtok(NULL, "|");
        strncpy(livro.titulo_livro, token ? token : "", MAX_TEXTO);

        token = strtok(NULL, "|");
        strncpy(livro.editora, token ? token : "", MAX_TEXTO);

        token = strtok(NULL, "|");
        livro.ano = token ? atoi(token) : 0;

        token = strtok(NULL, "|");
        livro.paginas = token ? atoi(token) : 0;

        fwrite(&livro, sizeof(Livro), 1, arquivoBin);
    }

    fclose(arquivoTxt);
    fclose(arquivoBin);
    printf("Importação concluída com sucesso!\n");
}

void buscaAutor(Autor *listaAutores, Autor *autor, Autor **out) {
    while (listaAutores != NULL &&
           (strcmp(listaAutores->nome, autor->nome) != 0 ||
            strcmp(listaAutores->sobrenome, autor->sobrenome) != 0)) {
        listaAutores = listaAutores->prox;
    }

    *out = listaAutores;
}

void inserirAutor(Autor **listaAutores, const char *nome, const char *sobrenome) {
    Autor *novo = (Autor *)malloc(sizeof(Autor));
    if (novo == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->nome, nome, MAX_TEXTO);
    strncpy(novo->sobrenome, sobrenome, MAX_TEXTO);
    novo->prox = NULL;

    Autor *aux;
    buscaAutor(*listaAutores, novo, &aux);
    if (aux != NULL) {
        printf("Autor já inserido\n");
        free(novo);
    } else {
        if (*listaAutores == NULL) {
            *listaAutores = novo;
        } else {
            aux = *listaAutores;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }
}

char isLetter(char character) {
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

void inserirAutores(Autor **listaAutores, const char *autores) {
    char nomesAutores[MAX_TEXTO][MAX_TEXTO];
    char sobreNomesAutores[MAX_TEXTO][MAX_TEXTO];
    int indiceLista = 0;
    int nomesAutoresLen = 0;
    int sobreNomesAutoresLen = 0;
    int autoresSize = strlen(autores);
    int flag = 1;

    memset(nomesAutores, 0, sizeof(nomesAutores));
    memset(sobreNomesAutores, 0, sizeof(sobreNomesAutores));

    for (int indiceString = 0; indiceString < autoresSize; indiceString++) {
        if (autores[indiceString] == ';') {
            indiceLista++;
            sobreNomesAutoresLen = 0;
            nomesAutoresLen = 0;
            flag = 1;
        } else if (autores[indiceString] == ',') {
            flag = !flag;
        } else if (isLetter(autores[indiceString]) || autores[indiceString] == '.' || autores[indiceString] == ' ') {
            if (flag) {
                nomesAutores[indiceLista][nomesAutoresLen++] = autores[indiceString];
            } else {
                sobreNomesAutores[indiceLista][sobreNomesAutoresLen++] = autores[indiceString];
            }
        }
    }

    for (int i = 0; i <= indiceLista; i++) {
        inserirAutor(listaAutores, nomesAutores[i], sobreNomesAutores[i]);
    }
}

void construirEstrutura(Editora **editora, Autor **autores) {
    FILE *arquivoBin = fopen("livros.dat", "rb");
    if (arquivoBin == NULL) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    Livro livro;
    while (fread(&livro, sizeof(Livro), 1, arquivoBin)) {
        inserirAutores(autores, livro.autores);
    }

    fclose(arquivoBin);
}

int main() {
    Editora *editoras = NULL;
    Autor *autores = NULL;

    importarLivros("livros.txt", "livros.dat");
    construirEstrutura(&editoras, &autores);

    Autor *current = autores;
    while (current != NULL) {
        printf("Nome:%s | Sobrenome:%s\n",current->nome,current->sobrenome);
        Autor *next = current->prox;
        free(current);
        current = next;
    }

    return 0;
}
