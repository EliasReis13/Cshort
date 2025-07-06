/**
 * @file main.c
 * @brief Ponto de entrada do compilador Cshort.
 *
 * @purpose
 * Este arquivo é o executável principal do compilador. Suas responsabilidades são:
 * 1. Processar os argumentos da linha de comando para obter o arquivo de código-fonte.
 * 2. Abrir o arquivo de código-fonte e disponibilizá-lo para os analisadores.
 * 3. Invocar o analisador sintático para iniciar o processo de compilação.
 * 4. Fechar o arquivo após a conclusão.
 * 5. Informar ao usuário o status final da compilação (sucesso ou erro).
 *
 * @note
 * Este arquivo depende da interface definida em "anaSint.h" para iniciar a análise.
 */

#include <stdio.h>
#include "anaSint.h" 

// --- Variáveis Globais ---
FILE *fd; // Ponteiro para o arquivo de código sendo compilado.
TOKEN t;  // Token atual (usado principalmente no analisador).
extern bool houveErroSintatico; // Flag que indica se ocorreram erros.


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
    // Verifica se um arquivo de entrada foi fornecido
    if (argc < 2) {
        printf("Uso: %s <arquivo.cshort>\n", argv[0]);
        return 1; // Retorna com código de erro
    }

    fd = fopen(argv[1], "r");
    if (!fd) {
        printf("Erro ao abrir arquivo: %s\n", argv[1]);
        return 1; // Retorna com código de erro
    }

    // Invoca a função inicial do analisador sintático
    Prog();

    fclose(fd);

    // Verifica a flag de erro para dar um feedback final ao usuário
    if (houveErroSintatico) {
        printf("\nAnalise sintatica concluida com erros.\n");
        return 1;
    }

    printf("\nAnalise sintatica concluida com sucesso!\n");
    return 0;
}