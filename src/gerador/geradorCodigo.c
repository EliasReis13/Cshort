#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geradorCodigo.h"

#define MAX_CODIGO 1000 // Número máximo de linhas de código que podem ser geradas
#define TAM_LINHA 100   // Tamanho máximo de uma única instrução

// Buffer para armazenar as instruções da máquina de pilha
static char codigo[MAX_CODIGO][TAM_LINHA];
static int linha_atual = 0;

// Contador para geração de rótulos únicos
static int contador_rotulo = 0;

// Adiciona uma instrução ao buffer de código
void gera(char *instrucao) {
    if (linha_atual < MAX_CODIGO) {
        strncpy(codigo[linha_atual], instrucao, TAM_LINHA - 1);
        codigo[linha_atual][TAM_LINHA - 1] = '\0'; // Garante terminação nula
        linha_atual++;
    } else {
        fprintf(stderr, "Erro: Limite de instrucoes do buffer de codigo excedido.\n");
    }
}

// Retorna um novo número de rótulo único
int novo_rotulo() {
    return contador_rotulo++;
}

// Gera a instrução de um rótulo e a adiciona ao buffer
void gera_rotulo(int r) {
    char rotulo[TAM_LINHA];
    snprintf(rotulo, sizeof(rotulo), "L%d:", r);
    gera(rotulo);
}

// Salva o buffer de instruções em um arquivo de texto
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