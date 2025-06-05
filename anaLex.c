
#include <stdbool.h>
#include <stdio.h>     // printf, fprintf, FILE, etc.
#include <stdlib.h>    // exit, atoi, atof
#include <string.h>    // strcpy, strcmp
#include <ctype.h>     // isspace, isprint, isalpha, isdigit

#include "anaLex.h"

int contLinha = 1; // Contador global de linhas

void errorLexico(char msg[]) {
    printf("\n\nErro lexico na linha %d: %s\n", contLinha, msg);
    exit(1);
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
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

TOKEN AnaLex(FILE *fd) {
    TOKEN token;
    int state = 0;
    char lexema[TAM_MAX_LEXEMA];
    char digitos_int[TAM_NUM];
    char digitos_real[TAM_NUM];
    int tamanho_digito = 0;
    int tamanho_digito_real = 0;
    int c, next;
    int i = 0;

    // Loop principal: continua até retornar um token
    while (true) {
        switch (state) {

            /***********************************/
            /* Estado 0: aguarda início do token */
            /***********************************/
            case 0:
                c = fgetc(fd);
                if (c == EOF) {
                    token.cat = END_FILE;
                    return token;
                }
                if (c == '\n') {
                    contLinha++;
                    state = 0;
                    break;
                }
                if (isspace(c)) {
                    state = 0;
                    break;
                }
                if (is_letter(c)) {
                    i = 0;
                    lexema[i++] = (char)c;
                    state = 1;
                    break;
                }
                if (is_digit(c)) {
                    tamanho_digito = 0;
                    digitos_int[tamanho_digito++] = (char)c;
                    state = 2;
                    break;
                }
                if (c == '\'') {
                    state = 5;
                    break;
                }
                if (c == '\"') {
                    state = 6;
                    break;
                }
                if (c == '/') {
                    next = fgetc(fd);
                    if (next == '*') {
                        state = 15;
                        break;
                    } else {
                        ungetc(next, fd);
                        token.cat = SN;
                        token.codigo = OP_DIVISAO;
                        return token;
                    }
                }
                if (c == '<') {
                    lexema[0] = '<'; i = 1;
                    state = 7;
                    break;
                }
                if (c == '>') {
                    lexema[0] = '>'; i = 1;
                    state = 8;
                    break;
                }
                if (c == '=') {
                    lexema[0] = '='; i = 1;
                    state = 9;
                    break;
                }
                if (c == '!') {
                    lexema[0] = '!'; i = 1;
                    state = 10;
                    break;
                }
                if (c == '&') {
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
                if (c == '|') {
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
                if (c == '+') {
                    token.cat = SN;
                    token.codigo = OP_SOMA;
                    return token;
                }
                if (c == '-') {
                    token.cat = SN;
                    token.codigo = OP_SUBTRACAO;
                    return token;
                }
                if (c == '*') {
                    token.cat = SN;
                    token.codigo = OP_MULTIPLICACAO;
                    return token;
                }
                if (c == '(') {
                    token.cat = SN;
                    token.codigo = ABRE_PARENTESES;
                    return token;
                }
                if (c == ')') {
                    token.cat = SN;
                    token.codigo = FECHA_PARENTESES;
                    return token;
                }
                if (c == '[') {
                    token.cat = SN;
                    token.codigo = ABRE_COLCHETE;
                    return token;
                }
                if (c == ']') {
                    token.cat = SN;
                    token.codigo = FECHA_COLCHETE;
                    return token;
                }
                if (c == '{') {
                    token.cat = SN;
                    token.codigo = ABRE_CHAVE;
                    return token;
                }
                if (c == '}') {
                    token.cat = SN;
                    token.codigo = FECHA_CHAVE;
                    return token;
                }
                if (c == ',') {
                    token.cat = SN;
                    token.codigo = VIRGULA;
                    return token;
                }
                if (c == ';') {
                    token.cat = END_EXPRESSION;
                    return token;
                }
                // Caracter inválido
                errorLexico("Caractere invalido");
                break;


            /**********************************************/
            /* Estado 1: identificador ou palavra reservada */
            /**********************************************/
            case 1:
                while (true) {
                    c = fgetc(fd);
                    if (is_letter(c) || is_digit(c)) {
                        if (i < TAM_MAX_LEXEMA - 1) {
                            lexema[i++] = (char)c;
                        } else {
                            // lexema muito grande: opcionalmente, chamar erro
                        }
                    } else {
                        // fim do identificador
                        ungetc(c, fd);
                        lexema[i] = '\0';
                        int codigoPR = reconhecerPalavraReservada(lexema);
                        if (codigoPR) {
                            token.cat = RESERVED_WORD;
                            token.codigo = codigoPR;
                        } else {
                            token.cat = ID;
                            strcpy(token.lexema, lexema);
                        }
                        return token;
                    }
                }
                break;


            /******************************************/
            /* Estado 2: constante inteira (possível real) */
            /******************************************/
            case 2:
                digitos_int[tamanho_digito] = '\0';
                c = fgetc(fd);
                if (is_digit(c)) {
                    if (tamanho_digito < TAM_NUM - 1) {
                        digitos_int[tamanho_digito++] = (char)c;
                        state = 2;
                    } else {
                        // overflow
                    }
                }
                else if (c == '.') {
                    tamanho_digito_real = 0;
                    state = 3;
                }
                else {
                    // termina inteiro
                    ungetc(c, fd);
                    digitos_int[tamanho_digito] = '\0';
                    token.cat = CT_INT;
                    token.int_value = atoi(digitos_int);
                    return token;
                }
                break;


            /*******************************/
            /* Estado 3: parte fracionária? */
            /*******************************/
            case 3:
                c = fgetc(fd);
                if (is_digit(c)) {
                    if (tamanho_digito_real < TAM_NUM - 1) {
                        digitos_real[tamanho_digito_real++] = (char)c;
                        state = 4;
                    } else {
                        // overflow
                    }
                } else {
                    errorLexico("Constante real mal formada");
                }
                break;


            /********************************************/
            /* Estado 4: continua parte fracionária real */
            /********************************************/
            case 4:
                digitos_real[tamanho_digito_real] = '\0';
                c = fgetc(fd);
                if (is_digit(c)) {
                    if (tamanho_digito_real < TAM_NUM - 1) {
                        digitos_real[tamanho_digito_real++] = (char)c;
                        state = 4;
                    } else {
                        // overflow
                    }
                }
                else {
                    // fim da real
                    ungetc(c, fd);
                    digitos_real[tamanho_digito_real] = '\0';
                    char buf[TAM_NUM * 2];
                    snprintf(buf, sizeof(buf), "%s.%s", digitos_int, digitos_real);
                    token.cat = CT_REAL;
                    token.real_value = atof(buf);
                    return token;
                }
                break;


            /******************************/
            /* Estado 5: literal de char */
            /******************************/
            case 5:
                c = fgetc(fd);
                if (c == '\\') {
                    next = fgetc(fd);
                    if (next == 'n' || next == '0') {
                        c = fgetc(fd);
                        if (c == '\'') {
                            token.cat = CT_CHAR;
                            if (next == 'n') token.int_value = '\n';
                            else token.int_value = '\0';
                            return token;
                        } else {
                            errorLexico("Char mal formado: falta ' final");
                        }
                    } else {
                        errorLexico("Char mal formado: sequência de escape inválida");
                    }
                }
                else if (isprint(c) && c != '\'' && c != '\\' && c != '\n') {
                    next = fgetc(fd);
                    if (next == '\'') {
                        token.cat = CT_CHAR;
                        token.int_value = (int)c;
                        return token;
                    } else {
                        errorLexico("Char mal formado: falta ' final");
                    }
                }
                else {
                    errorLexico("Char constante inválida");
                }
                break;


            /*******************************/
            /* Estado 6: literal de string */
            /*******************************/
            case 6:
                i = 0;
                while (true) {
                    c = fgetc(fd);
                    if (c == EOF || c == '\n') {
                        errorLexico("String não fechada");
                    }
                    if (c == '\"') {
                        lexema[i] = '\0';
                        token.cat = CT_STRING;
                        strcpy(token.lexema, lexema);
                        return token;
                    }
                    if (c != '"' && c != '\n' && c != EOF) {
                        if (i < TAM_MAX_LEXEMA - 1) {
                            lexema[i++] = (char)c;
                        } else {
                            // string muito comprida
                        }
                    } else {
                        errorLexico("Caractere invalido dentro de string");
                    }
                }
                break;


            /**************************************/
            /* Estado 7: operador < ou <= */
            /**************************************/
            case 7:
                c = fgetc(fd);
                if (c == '=') {
                    token.cat = SN;
                    token.codigo = OP_MENOR_IGUAL;
                    return token;
                } else {
                    ungetc(c, fd);
                    token.cat = SN;
                    token.codigo = OP_MENOR;
                    return token;
                }
                break;


            /**************************************/
            /* Estado 8: operador > ou >= */
            /**************************************/
            case 8:
                c = fgetc(fd);
                if (c == '=') {
                    token.cat = SN;
                    token.codigo = OP_MAIOR_IGUAL;
                    return token;
                } else {
                    ungetc(c, fd);
                    token.cat = SN;
                    token.codigo = OP_MAIOR;
                    return token;
                }
                break;


            /**************************************/
            /* Estado 9: operador = ou == */
            /**************************************/
            case 9:
                c = fgetc(fd);
                if (c == '=') {
                    token.cat = SN;
                    token.codigo = OP_IGUAL;
                    return token;
                } else {
                    ungetc(c, fd);
                    token.cat = SN;
                    token.codigo = OP_ATRIBUICAO;
                    return token;
                }
                break;


            /**************************************/
            /* Estado 10: operador ! ou != */
            /**************************************/
            case 10:
                c = fgetc(fd);
                if (c == '=') {
                    token.cat = SN;
                    token.codigo = OP_DIFERENTE;
                    return token;
                } else {
                    ungetc(c, fd);
                    token.cat = SN;
                    token.codigo = OP_NOT;
                    return token;
                }
                break;


            /**************************************/
            /* Estado 15: comentário de bloco */ 
            /**************************************/
            case 15:
                while (true) {
                    c = fgetc(fd);
                    if (c == EOF) {
                        errorLexico("Comentário não fechado antes do EOF");
                    }
                    if (c == '\n') {
                        contLinha++;
                    }
                    if (c == '*') {
                        next = fgetc(fd);
                        if (next == '/') {
                            // fim do comentário: volta ao estado 0
                            state = 0;
                            break;
                        } else {
                            ungetc(next, fd);
                            // continua dentro do comentário
                            state = 15;
                        }
                    }
                    // senão, continua dentro do comentário
                }
                break;

            default:
                errorLexico("Estado inesperado no analisador lexico");
                break;
        }
        // fim do switch(state)
        // se não retornamos token, voltamos ao topo do while(true) e
        // consultamos o case(state) atualizado (em geral, state=0 após comentário)
    }
    // fim do while(true)
}
