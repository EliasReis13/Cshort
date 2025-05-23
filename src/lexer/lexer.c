#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "keywords.h"
#include "../../include/lexer.h"
#include "../../../include/lexer.h"

static FILE *source;
static int current_char;
static int line = 1, column = 0;

void init_lexer(const char *filename) {
    source = fopen(filename, "r");
    if (!source) {
        perror("Erro ao abrir arquivo");
        return;
    }
    current_char = fgetc(source);
}

void close_lexer() {
    if (source) fclose(source);
}

static void advance(){
    if (current_char == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
    current_char = fgetc(source);
}

Token next_token() {
    Token token;
    token.lexeme[0] = '\0';
    token.line = line;
    token.column = column;

    while (isspace(current_char)) {
        advance();
    }

    if (isalpha(current_char)) {
        int i = 0; 
        while (isalnum(current_char) || current_char == '_'){
            token.lexeme[i++] = current_char;
            advance();
        }

        token.lexeme[i] = '\0';
        token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_ID;
        return token;
    }

    if (isdigit(current_char)){
        int i = 0;
        while (isdigit(current_char)){
            token.lexeme[i++] = current_char;
            advance();
        }

        if (current_char == '.') {
            token.lexeme[i++] = current_char;
            advance();
            while (isdigit(current_char)) {
                token.lexeme[i++] = current_char;
                advance();
            }
            token.lexeme[i] = '\0';
            token.type = TOKEN_REALCON;
        } else {
            token.lexeme[i] = '\0';
            token.type = TOKEN_INTCON;
        }
        return token;
    }

    if (current_char == '\'') {
        advance();
        if (current_char == '\\') {
            advance();
            if current_char == 'n' || current_char == '0'){
                sprintf(token.lexeme, "'\\%c'", current_char);
                advance();
            } else {
                token.type = TOKEN_UNKNOWN;
                return token;
            }
        } else if (isprint(current_char) && current_char != '\'' && current_char != '\\'){
            token.lexeme[0] = '\'';
            token.lexeme[1] = current_char;
            token.lexeme[2] = '\'';
            token.lexeme[3] = '\0';
            advance();
        } else {
            token.type = TOKEN_UNKNOWN
            return token;
        }

        if (current_char != '\'') {
            advance();
            token.type = TOKEN_CHARCON;
            return token;
        }

        token.type = TOKEN_UNKNOWN;
        return token;
    }
    
    if (current_char == '"'){
        int i = 0;
        token.lexeme[i++] = current_char;
        advance();
        while (isprint(current_char) && current_char != '"' && current_char != '\n'){
            token.lexeme[i++] = current_char;
            advance();
        }
        if (current_char == '"'){
            token.lexeme[i++] = current_char;
            token.lexeme[i] = '\0';
            advance();
            token.type = TOKEN_STRINGCON;
        } else {
            token.type = TOKEN_UNKNOWN;
        }
        return token;
    }

    if (current_char == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    char c = current_char;
    advance();
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.type = ispunct(c) ? TOKEN_OPERATOR : TOKEN_UNKNOWN;
    return token;
}