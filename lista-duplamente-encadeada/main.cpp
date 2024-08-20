#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 1024
#define MAX_TEXTO 256

struct autor {
	struct autor *prox;
	char nome[MAX_TEXTO],sobrenome[MAX_TEXTO];
}; typedef struct autor Autor;

struct  listaAutor {
	struct autor *autor;
	struct  listaAutor *prox;
};

struct livros {
	struct livros *ant,*prox;
	struct listaAutor *lista_autores;
	char titulo[100];
	int ano,paginas;
};

struct editora {
	struct livros *livros;
	char nome[100];
}; typedef struct  editora Editora;


// Estrutura para armazenar os dados do livro
typedef struct {
	char autores[MAX_TEXTO];
	char titulo_livro[MAX_TEXTO];
	char editora[MAX_TEXTO];
	int ano;
	int paginas;
} Livro;

void importarLivros(const char *arquivoTexto, const char *arquivoBinario) {
	printf("%s", arquivoTexto);
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
		// Ler autores
		char *token = strtok(linha, "|");

		strncpy(livro.autores, token, MAX_TEXTO);

		// Ler título do livro
		token = strtok(NULL, "|");
		strncpy(livro.titulo_livro, token, MAX_TEXTO);

		// Ler editora
		token = strtok(NULL, "|");
		strncpy(livro.editora, token, MAX_TEXTO);

		// Ler ano
		token = strtok(NULL, "|");
		livro.ano = atoi(token);

		// Ler número de páginas
		token = strtok(NULL, "|");
		livro.paginas = atoi(token);

		// Escrever no arquivo binário
		fwrite(&livro, sizeof(Livro), 1, arquivoBin);
	}

	fclose(arquivoTxt);
	fclose(arquivoBin);
	printf("Importação concluída com sucesso!\n");
}

void buscaAutor(Autor *listaAutores,Autor *autor, Autor **out) {
	while(listaAutores != NULL && (strcmp(listaAutores->nome,autor->nome) != 0 || strcmp(listaAutores->sobrenome,autor->sobrenome) != 0)) {
		listaAutores = listaAutores->prox;
	}

	*out = listaAutores;
}


void inserirAutor(Autor **listaAutores,char *nome,char *sobrenome) {
	Autor *novo = (Autor*) malloc(sizeof(Autor*));
	strcpy(novo->nome,nome);
	strcpy(novo->sobrenome,sobrenome);
	novo->prox = NULL;

	if(*listaAutores == NULL)
		*listaAutores = novo;
	else {
		Autor *aux;
		buscaAutor(*listaAutores,novo,&aux);
		if(aux != NULL) {
			printf("Autor já inserido");
		}
		else {
			aux = *listaAutores;
			while (aux->prox != NULL) {
				aux=aux->prox;
			}
			aux->prox = novo;
		}
	}
}

char isLetter(char character) {
	return character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z';
}

void inserirAutores(Autor **listaAutores,char *autores) {
	char nomesAutores[MAX_TEXTO][MAX_TEXTO];
	int nomesAutoresLen = 0;
	char sobreNomesAutores[MAX_TEXTO][MAX_TEXTO];
	int sobreNomesAutoresLen = 0;
	int autoresSize = strlen(autores);
	int indiceLista=0;
	memset(nomesAutores, 0, sizeof(nomesAutores));
	memset(sobreNomesAutores, 0, sizeof(sobreNomesAutores));
	char flag = 1;
	for(int indiceString = 0;indiceString<autoresSize;indiceString++) {

		if(autores[indiceString] == ';') {
			indiceLista++;
			sobreNomesAutoresLen = 0;
			nomesAutoresLen = 0;
			flag = 1;
		}

		if(autores[indiceString] == ',') {
			if(flag)
				flag = 0;
			else
				flag = 1;
		}

		if(isLetter(autores[indiceString]) || autores[indiceString] == '.' || autores[indiceString] == ' ') {
			if(flag) {
				nomesAutores[indiceLista][nomesAutoresLen] = autores[indiceString];
				nomesAutoresLen++;
			}
			else {
				sobreNomesAutores[indiceLista][sobreNomesAutoresLen] = autores[indiceString];
				sobreNomesAutoresLen++;
			}
		}
	}

	for (int i = 0; i < indiceLista +1; i++) {
		inserirAutor(listaAutores,nomesAutores[i],sobreNomesAutores[i]);
	}

	while (*listaAutores != NULL) {
		printf("Nome:%s | Sobrenome:%s\n", (*listaAutores)->nome,(*listaAutores)->sobrenome);
		*listaAutores = (*listaAutores)->prox;
	}
}

void construirEstrutura(Editora **editora,Autor **autores) {
	FILE *arquivoBin = fopen("livros.dat", "rb");
	if (arquivoBin == NULL) {
		perror("Erro ao abrir o arquivo binário");
	}

	Livro livro;
	while(fread(&livro,sizeof(Livro),1,arquivoBin)) {
		inserirAutores(autores,livro.autores);
	}

}

int main() {
	Editora *editoras = NULL;
	Autor *autores = NULL;

	importarLivros("livros.txt", "livros.dat");
	construirEstrutura(&editoras,&autores);

	return 0;
}
