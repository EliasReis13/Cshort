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
                    // ignora espaços, tabs, etc.
                    state = 0;
                    break;
                }
                if (is_letter(c)) {
                    // começa identificador ou palavra reservada
                    i = 0;
                    lexema[i++] = (char)c;
                    state = 1;
                    break;
                }
                if (is_digit(c)) {
                    // começa constante inteira ou real
                    tamanho_digito = 0;
                    digitos_int[tamanho_digito++] = (char)c;
                    state = 2;
                    break;
                }
                if (c == '\'') {
                    // começa literal de caractere
                    state = 5;
                    break;
                }
                if (c == '\"') {
                    // começa literal de string
                    state = 6;
                    break;
                }
                if (c == '/') {
                    next = fgetc(fd);
                    if (next == '*') {
                        // comentário
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
                // se não casou nada:
                errorLexico("Caractere inválido");
                break;


            case 1: // Reconhecendo identificadores ou palavras reservadas
                while (true) {
                    c = fgetc(fd);
                    if (is_letter(c) || is_digit(c)) {
                        if (i < TAM_MAX_LEXEMA - 1) {
                            lexema[i++] = (char)c;
                        } else {
                            errorLexico("Lexema muito longo");
                        }
        
                    } else {
                        ungetc(c, fd); // Coloca o caractere de volta no buffer
                        lexema[i] = '\0'; // Termina o lexema
                        
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

            case 2: // Reconhecendo Número Inteiro ou Real
                digitos_int[tamanho_digito++] = '\0';
                c = fgetc(fd);

                if (is_digit(c)) {

                    if (tamanho_digito < TAM_NUM - 1) {
                        digitos_int[tamanho_digito++] = (char)c;
                        state = 2; 
                    } else {
                        errorLexico("Número inteiro muito longo");
                    }
                }
                else if (c == '.') {
                    tamanho_digito_real = 0;
                    state = 3;
                }

                else {
                    ungetc(c, fd); // Coloca o caractere de volta no buffer
                    digitos_int[tamanho_digito] = '\0'; // Termina o número inteiro
                    token.cat = CT_INT;
                    token.int_value = atoi(digitos_int);
                    return token;
                }
                break;

            case 3: // Estado após o ponto decimal: espera pelo menos um dígito
                c = fgetc(fd);

                if (is_digit(c)) {
                    if (tamanho_digito_real < TAM_NUM - 1) {
                        digitos_real[tamanho_digito_real++] = (char)c;
                        state = 4;
                    } else{
                        //estouro
                    }
                    
                } else {
                    errorLexico("Esperava dígito após o ponto decimal");
                }
                break;	
            
            case 4: // Ler dígitos de parte fracionária
                digitos_real[tamanho_digito_real] = '\0';
                c = fgetc(fd);

               if (is_digit(c)) {
                    if (tamanho_digito_real < TAM_NUM - 1) {
                        digitos_real[tamanho_digito_real++] = (char)c;
                        state = 4;
                    } else {
                        // estouro
                    }
                }
                
                else{ // termina constate real
                    ungetc(c, fd); 
                    digitos_real[tamanho_digito_real] = '\0';

                    //monta string completa: "digitos_int.digitos_real"
                    char buf[TAM_NUM * 2];
                    snprintf(buf, sizeof(buf), "%s.%s", digitos_int, digitos_real);
                    token.cat = CT_REAL;
                    token.real_value = atof(buf);
                    return token;
                }
                break;
        }   
    }
    
}