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
    if (argc < 2) {
        printf("Uso: %s <arquivo.cshort>\n", argv[0]);
        return 1;
    }
    Prog(argv[1]); 
    
    if (houveErroSintatico) {
        printf("\nAnalise concluida com erros.\n");
        return 1;
    }
    printf("\nAnalise concluida com sucesso!\n");
    return 0;
}
