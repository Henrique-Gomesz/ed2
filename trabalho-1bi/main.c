#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 150

typedef struct token {
    struct token *next;
    char *value;
} Token;

typedef struct program {
    struct program *nextLine, *prevLine;
    Token *tokens;
} Program;

void insertToken(Token **tokens, const char *value) {
    Token *newToken = (Token *)malloc(sizeof(Token));
    newToken->value = strdup(value);  // Duplicate the token string
    newToken->next = NULL;

    if (*tokens) {
        Token *aux = *tokens;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = newToken;
    } else {
        *tokens = newToken;
    }
}

char isLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

char isDigit(char c) {
    return (c >= '0' && c <= '9');
}

char isOperatorOrPunctuation(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '=' || c == '(' || c == ')' || c == '\'' ||
            c == ',' || c == '.' || c == ';' || c == '"');
}

Token *breakLineIntoTokens(const char *line) {
    Token *tokens = NULL;
    char aux[MAX_LINE_LENGTH];
    int auxLength = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (isspace(line[i]) || isOperatorOrPunctuation(line[i])) {
            if (auxLength > 0) {
                aux[auxLength] = '\0';
                insertToken(&tokens, aux);
                auxLength = 0;
            }
            if (isOperatorOrPunctuation(line[i])) {
                aux[0] = line[i];
                aux[1] = '\0';
                insertToken(&tokens, aux);
            }
        } else {
            aux[auxLength++] = line[i];
        }
    }
    if (auxLength > 0) {
        aux[auxLength] = '\0';
        insertToken(&tokens, aux);
    }
    return tokens;
}

void insertProgramLine(Program **programs, const char *line) {
    Program *newProgramLine = (Program *)malloc(sizeof(Program));
    newProgramLine->nextLine = NULL;
    newProgramLine->prevLine = NULL;
    newProgramLine->tokens = breakLineIntoTokens(line);

    if (*programs) {
        Program *aux = *programs;
        while (aux->nextLine != NULL)
            aux = aux->nextLine;
        aux->nextLine = newProgramLine;
        newProgramLine->prevLine = aux;
    } else {
        *programs = newProgramLine;
    }
}

void printTokens(Token const *tokens) {
    while (tokens) {
        printf("Token: %s\n", tokens->value);
        tokens = tokens->next;
    }
}

void printProgram(Program const *program) {
    while (program) {
        printTokens(program->tokens);
        printf("\n");
        program = program->nextLine;
    }
}

void loadProgramFromFile(const char *filename, Program **programs) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        insertProgramLine(programs, line);
    }

    fclose(file);
}

int main(void) {
    Program *program = NULL;

    loadProgramFromFile("code.txt", &program);

    printProgram(program);

    return 0;
}
