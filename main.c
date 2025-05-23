#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "../include/lexer.h"
#include "../include/keywords.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo-fonte>\n", argv[0]);
        return 1;
    }

    init_keywords(); // Inicializa a hash table de palavras-chave
    init_lexer(argv[1]); // Inicializa o lexer com o arquivo fonte

    Token token;

    printf("=== Tokens Reconhecidos ===\n");

    do{
        token = next_token();

        const char *type_str = "";
        switch (token.type) {
            case TOKEN_ID:        type_str = "ID"; break;
            case TOKEN_INTCON:    type_str = "INTCON"; break;
            case TOKEN_REALCON:   type_str = "REALCON"; break;
            case TOKEN_CHARCON:   type_str = "CHARCON"; break;
            case TOKEN_STRINGCON: type_str = "STRINGCON"; break;
            case TOKEN_KEYWORD:   type_str = "KEYWORD"; break;
            case TOKEN_OPERATOR:  type_str = "OPERATOR"; break;
            case TOKEN_DELIMITER: type_str = "DELIMITER"; break;
            case TOKEN_COMMENT:   type_str = "COMMENT"; break;
            case TOKEN_EOF:       type_str = "EOF"; break;
            default:              type_str = "UNKNOWN"; break;
        }

        printf("Token: %-12s | Lexema: %-15s | Linha: %d | Coluna: %d\n",
               type_str, token.lexeme, token.line, token.column);

    } while (token.type != TOKEN_EOF);

    close_lexer(); // Fecha o lexer

    return 0;
}