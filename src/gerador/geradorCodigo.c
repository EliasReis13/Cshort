/**
 * @file geradorCodigo.c
 * @brief Implementação do Módulo Gerador de Código.
 *
 * @purpose
 * Este arquivo contém a lógica para gerar e armazenar o código intermediário
 * para uma máquina de pilha. As instruções são guardadas em um buffer estático
 * na memória e, ao final da compilação, são salvas em um arquivo de saída.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geradorCodigo.h"

#define MAX_CODIGO 1000 // Número máximo de linhas de código que podem ser geradas

// Buffer para armazenar as instruções da máquina de pilha
static char codigo[MAX_CODIGO][TAM_LINHA];
static int linha_atual = 0;

// Contador para geração de rótulos únicos
static int contador_rotulo = 0;

/**
 * @brief Adiciona uma instrução ao buffer de código.
 * Utiliza strncpy para copiar a instrução e evitar overflow.
 */

void gera(char *instrucao) {
    if (linha_atual < MAX_CODIGO) {
        strncpy(codigo[linha_atual], instrucao, TAM_LINHA - 1);
        codigo[linha_atual][TAM_LINHA - 1] = '\0'; // Garante que a string seja terminada com nulo
        linha_atual++;
    } else {
        fprintf(stderr, "Erro: Limite de instrucoes do buffer de codigo excedido.\n");
    }
}

/**
 * @brief Retorna um novo número de rótulo único.
 * A cada chamada, incrementa o contador global de rótulos.
 */

int novo_rotulo() {
    return contador_rotulo++;
}

/**
 * @brief Gera a instrução de um rótulo (ex: "L1:") e a adiciona ao buffer.
 * Formata a string antes de passá-la para a função 'gera'.
 */

void gera_rotulo(int r) {
    char rotulo[TAM_LINHA];
    snprintf(rotulo, sizeof(rotulo), "L%d:", r);
    gera(rotulo);
}

/**
 * @brief Salva o buffer de instruções em um arquivo de texto.
 * Percorre o buffer 'codigo' e escreve cada instrução em uma nova linha no arquivo.
 */

 void salvar_codigo_em_arquivo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de saida do codigo gerado");
        return;
    }

    for (int i = 0; i < linha_atual; i++) {
        fprintf(arquivo, "%s\n", codigo[i]);
    }

    fclose(arquivo);
    printf("Codigo de maquina de pilha salvo em: %s\n", nome_arquivo);
}