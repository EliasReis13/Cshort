#include <stdio.h>
#include "lex/anaLex.h"

/*
 * Função principal do compilador Cshort.
 * Por enquanto, executa apenas a análise léxica do arquivo fonte.
 *
 * Uso:
 *     ./cshort.exe <arquivo.cshort>
 *
 * Etapas:
 * 1. Verifica argumentos e abre arquivo fonte.
 * 2. Chama a função AnaLex() repetidamente para ler tokens.
 * 3. Imprime informações dos tokens na tela e no arquivo tokens.txt.
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.cshort>\n", argv[0]);
        return 1;
    }

    // Abre arquivo fonte especificado na linha de comando
    FILE *fd = fopen(argv[1], "r");
    if (!fd) {
        printf("Erro ao abrir arquivo: %s\n", argv[1]);
        return 1;
    }

    // Abre arquivo de saída para gravação dos tokens reconhecidos
    FILE *out = fopen("out/tokens.txt", "w");
    if (!out) {
        printf("Erro ao abrir tokens.txt\n");
        return 1;
    }

    TOKEN t;
    do {
        // Obtém o próximo token do arquivo fonte
        t = AnaLex(fd);
        printf("Token: cat=%d", t.cat);

        // Imprime detalhes do token, de acordo com sua categoria
        switch (t.cat) {
            case ID:
                printf(" (ID, lexema=\"%s\")", t.lexema);
                fprintf(out, "<ID, %s>\n", t.lexema);
                break;
            case RESERVED_WORD:
                printf(" (PR, codigo=%d)", t.codigo);
                fprintf(out, "<PR, %d>\n", t.codigo);
                break;
            case CT_INT:
                printf(" (INT, valor=%d)", t.int_value);
                fprintf(out, "<CT_INT, %d>\n", t.int_value);
                break;
            case CT_REAL:
                printf(" (REAL, valor=%.2f)", t.real_value);
                fprintf(out, "<CT_REAL, %.2f>\n", t.real_value);
                break;
            case CT_CHAR:
                printf(" (CHAR, valor=%d)", t.int_value);
                fprintf(out, "<CT_CHAR, '%c'>\n", t.int_value);
                break;
            case CT_STRING:
                printf(" (STRING, lexema=\"%s\")", t.lexema);
                fprintf(out, "<CT_STRING, \"%s\">\n", t.lexema);
                break;
            case SN:
                printf(" (SINAL, codigo=%d)", t.codigo);
                fprintf(out, "<SN, %d>\n", t.codigo);
                break;
            case END_EXPRESSION:
                printf(" (;)");
                fprintf(out, "<END_EXPR, ;>\n");
                break;
            case COMENT:
                // Comentário ignorado, não precisa imprimir
                break;
            case END_FILE:
                printf(" (EOF)");
                fprintf(out, "<EOF>\n");
                break;
        }
        printf("\n");
    } while (t.cat != END_FILE);

    // Fecha arquivos ao final da execução
    fclose(fd);
    fclose(out);
    return 0;
}
