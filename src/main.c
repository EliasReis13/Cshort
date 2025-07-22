/**
 * @file main.c
 * @brief Ponto de entrada do compilador Cshort.
*/

#include <stdio.h>
#include "sint/anaSint.h"

/**
 * @brief Função principal do compilador.
 *
 * @param argc Número de argumentos da linha de comando.
 * @param argv Vetor de strings contendo os argumentos. Espera-se que argv[1]
 * seja o caminho para o arquivo .cshort a ser compilado.
 *
 * @return int Retorna 0 em caso de sucesso e 1 em caso de erro.
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

