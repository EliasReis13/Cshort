#include <stdio.h>
#include "anaLex.h"

/*
 * Programa principal do analisador léxico.
 * Abre o arquivo "teste.cshort" para leitura e utiliza a função AnaLex
 * para identificar e imprimir os tokens encontrados no arquivo.
 *
 * Fluxo:
 * 1. Abre o arquivo de entrada.
 * 2. Enquanto não encontrar o fim do arquivo, chama AnaLex para obter o próximo token.
 * 3. Imprime informações detalhadas sobre cada token identificado.
 * 4. Fecha o arquivo ao final.
 */

int main() {
    FILE *fd = fopen("test/teste_erro1.cshort", "r"); // Abre o arquivo de entrada para leitura
    if (!fd) {
        printf("Nao consegui abrir arquivo.\n"); // Mensagem de erro se não conseguir abrir
        return 1;
    }
    FILE *out = fopen("out/tokens.txt", "w");
    if (!out){
        printf("Erro ao abrir tokens.txt\n");
        return 1;
    }

    TOKEN t;
    do {
        t = AnaLex(fd); // Chama o analisador léxico para obter o próximo token
        printf("Token: cat=%d", t.cat); // Imprime a categoria do token
        switch (t.cat) {
            case ID: // Token identificador (variáveis, nomes de função, etc)
                printf(" (ID, lexema=\"%s\")", t.lexema);
                fprintf(out, "<ID, %s>\n", t.lexema);
                break;

            case RESERVED_WORD: // Palavra reservada da linguagem (if, int, while, etc)
                printf(" (PR, codigo=%d)", t.codigo);
                fprintf(out, "<PR, %d>\n", t.codigo);
                break;

            case CT_INT: // Constante inteira (ex: 10, 42)
                printf(" (INT, valor=%d)", t.int_value);
                fprintf(out, "<CT_INT, %d>\n", t.int_value);
                break;

            case CT_REAL: // Constante real (ex: 3.14, 20.0)
                printf(" (REAL, valor=%.2f)", t.real_value);
                fprintf(out, "<CT_REAL, %.2f>\n", t.real_value);
                break;

            case CT_CHAR: // Constante caractere (ex: 'a') — valor armazenado como inteiro (ASCII)
                printf(" (CHAR, valor=%d)", t.int_value);
                fprintf(out, "<CT_CHAR, '%c'>\n", t.int_value);
                break;

            case CT_STRING: // Constante string (ex: "Olá Mundo")
                printf(" (STRING, lexema=\"%s\")", t.lexema);
                fprintf(out, "<CT_STRING, \"%s\">\n", t.lexema);
                break;

            case SN: // Sinal: operadores (ex: =, +, -, ==) ou símbolos de pontuação
                printf(" (SINAL, codigo=%d)", t.codigo);
                fprintf(out, "<SN, %d>\n", t.codigo);
                break;

            case END_EXPRESSION: // Ponto e vírgula indicando fim de expressão
                printf(" (;)");
                fprintf(out, "<END_EXPR, ;>\n");
                break;

            case END_FILE: // Fim do arquivo fonte
                printf(" (EOF)");
                fprintf(out, "<EOF>\n");
                break;
            default:
                // Categoria desconhecida ou erro
                break;
        }
        printf("\n");
    } while (t.cat != END_FILE); // Repete até o fim do arquivo

    fclose(fd); // Fecha o arquivo
    fclose(out);
    return 0;
}