#include <stdbool.h>
#include "anaLex.h"

int contLinha = 1; // Inicializa o contador de linhas

void errorLexico(char msg[]) {
    printf("Erro léxico na linha %d: %s\n", contLinha, msg);
    exit(1); // Encerra o programa

}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int reconhecerPalavraReservada(char lexema[]) {
    if (strcmp(lexema, "if") == 0)       return PR_IF;
    if (strcmp(lexema, "else") == 0)     return PR_ELSE;
    if (strcmp(lexema, "int") == 0)      return PR_INT;
    if (strcmp(lexema, "float") == 0)    return PR_FLOAT;
    if (strcmp(lexema, "char") == 0)     return PR_CHAR;
    if (strcmp(lexema, "string") == 0)   return PR_STRING;
    if (strcmp(lexema, "while") == 0)    return PR_WHILE;
    if (strcmp(lexema, "for") == 0)      return PR_FOR;
    if (strcmp(lexema, "return") == 0)   return PR_RETURN;
    if (strcmp(lexema, "break") == 0)    return PR_BREAK;
    if (strcmp(lexema, "continue") == 0) return PR_CONTINUE;
    return 0;
}

TOKEN AnaLex(FILE *fd){
    TOKEN token;
    int state = 0;
    char lexema[TAM_MAX_LEXEMA];
    char digitos_int[TAM_NUM] = "";
    int tamanho_digito = 0;
    char digitos_real[TAM_NUM] = "";
    int tamanho_digito_real = 0;
    int c, next;
    int i = 0;

    while (true) {
        c = fgetc(fd);
        if (c == '\n') {
            contLinha++;
        }

        switch (state){
            case 0:
                if (c == EOF) {
                    token.cat = END_FILE;
                    return token;
                }
                if (isspace(c)) {
                    state = 0;
                }
                else if (is_letter(c)) {
                    lexema[i++] = (char)c;
                    state = 1;
                }
                else if (is_digit(c)) {
                    digitos_int[tamanho_digito++] = (char)c;
                    state = 2;
                }
                else if (c == '\'') {
                    state = 5;
                }
                else if (c == '\"') {
                    state = 6;
                }
                else if (c == '/') {
                    next = fgetc(fd);
                    if (next == '*') {
                        state = 15;
                    } else {
                        ungetc(next, fd);
                        token.cat = SN;
                        token.codigo = OP_DIVISAO;
                        return token;
                    }
                }
                else if (c == '<') {
                    lexema[i++] = (char)c;
                    state = 7;
                }
                else if (c == '>') {
                    lexema[i++] = (char)c;
                    state = 8;
                }
                else if (c == '=') {
                    lexema[i++] = (char)c;
                    state = 9;
                }
                else if (c == '!') {
                    lexema[i++] = (char)c;
                    state = 10;
                }
                else if (c == '&') {
                    next = fgetc(fd);
                    if (next == '&') {
                        token.cat = SN;
                        token.codigo = OP_AND;
                        return token;
                    } else {
                        ungetc(next, fd);
                        errorLexico("Operador '&' isolado");
                    }
                }
                else if (c == '|') {
                    next = fgetc(fd);
                    if (next == '|') {
                        token.cat = SN;
                        token.codigo = OP_OR;
                        return token;
                    } else {
                        ungetc(next, fd);
                        errorLexico("Operador '|' isolado");
                    }
                }
                else if (c == '+') {
                    token.cat = SN;
                    token.codigo = OP_SOMA;
                    return token;
                }
                else if (c == '-') {
                    token.cat = SN;
                    token.codigo = OP_SUBTRACAO;
                    return token;
                }
                else if (c == '*') {
                    token.cat = SN;
                    token.codigo = OP_MULTIPLICACAO;
                    return token;
                }
                else if (c == '(') {
                    token.cat = SN;
                    token.codigo = ABRE_PARENTESES;
                    return token;
                }
                else if (c == ')') {
                    token.cat = SN;
                    token.codigo = FECHA_PARENTESES;
                    return token;
                }
                else if (c == '[') {
                    token.cat = SN;
                    token.codigo = ABRE_COLCHETE;
                    return token;
                }
                else if (c == ']') {
                    token.cat = SN;
                    token.codigo = FECHA_COLCHETE;
                    return token;
                }
                else if (c == '{') {
                    token.cat = SN;
                    token.codigo = ABRE_CHAVE;
                    return token;
                }
                else if (c == '}') {
                    token.cat = SN;
                    token.codigo = FECHA_CHAVE;
                    return token;
                }
                else if (c == ',') {
                    token.cat = SN;
                    token.codigo = VIRGULA;
                    return token;
                }
                else if (c == ';') {
                    token.cat = END_EXPRESSION;
                    return token;
                }
                else {
                    errorLexico("Caractere inválido");
                }
                break;
        }   
    }
    
}