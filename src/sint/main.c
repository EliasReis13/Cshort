#include <stdio.h>
#include "anaSint.h"

FILE *fd;
TOKEN t;
// A variável houveErroSintatico é declarada em anaSint.c e visível aqui via anaSint.h

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.cshort>\n", argv[0]);
        return 1;
    }

    fd = fopen(argv[1], "r");
    if (!fd) {
        printf("Erro ao abrir arquivo: %s\n", argv[1]);
        return 1;
    }

    // Inicia a análise sintática
    Prog();

    // Fecha o arquivo ao final
    fclose(fd);

    if (houveErroSintatico) {
        printf("\nAnalise sintatica concluida com erros.\n");
        return 1; // Retorna um código de erro para o sistema operacional
    }

    printf("\nAnalise sintatica concluida com sucesso!\n");
    return 0; // Retorna sucesso
}