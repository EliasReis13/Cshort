#include <stdio.h>
#include "sint/anaSint.h" // Inclui o cabeçalho do SINTÁTICO
#include "lex/anaLex.h"   // Inclui o cabeçalho do LÉXICO

/*
 * Programa principal que executa o ANALISADOR SINTÁTICO para a linguagem Cshort.
 * * Fluxo:
 * 1. Abre o arquivo de entrada.
 * 2. Chama a função Prog() do analisador sintático, que é o ponto de partida da análise.
 * 3. Fecha o arquivo ao final.
 */

int main(int argc, char *argv[]) {
    // Verifica se um nome de arquivo foi passado como argumento
    if (argc < 2) {
        printf("Uso: %s <arquivo_fonte.cshort>\n", argv[0]);
        return 1;
    }

    // Tenta abrir o arquivo passado como argumento
    fd = fopen(argv[1], "r"); 
    if (!fd) {
        printf("Nao consegui abrir o arquivo '%s'.\n", argv[1]);
        return 1;
    }

    // Chama a função inicial do Analisador Sintático
    Prog();

    printf("\nAnalise sintatica concluida.\n");
    if (houveErroSintatico) {
        printf("Foram encontrados erros sintaticos.\n");
    } else {
        printf("Nenhum erro sintatico encontrado.\n");
    }

    fclose(fd); // Fecha o arquivo
    return 0;
}