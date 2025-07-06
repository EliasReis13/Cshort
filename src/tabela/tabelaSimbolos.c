// /tabela/tabelaSimbolos.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabelaSimbolos.h"

extern int contLinha; // Importa o contador de linha do analisador léxico/sintático
bool houveErroSemantico = false;

void erro_semantico(char* msg, int linha) {
    printf("[ERRO SEMANTICO | Linha %d]: %s\n", linha, msg);
    houveErroSemantico = true;
}

TabelaSimbolos inicializa_tabela() {
    TabelaSimbolos ts;
    ts.tamanho = 0;
    ts.escopo_atual = 0; // Escopo global
    return ts;
}

void abre_escopo(TabelaSimbolos *ts) {
    ts->escopo_atual++;
}

void fecha_escopo(TabelaSimbolos *ts) {
    // Remove todos os símbolos do escopo que está sendo fechado
    int i = ts->tamanho - 1;
    while (i >= 0 && ts->simbolos[i].nivel_lexico == ts->escopo_atual) {
        ts->tamanho--;
        i--;
    }
    ts->escopo_atual--;
}

// Busca por um símbolo apenas no escopo atual
SIMBOLO* busca_escopo_atual(TabelaSimbolos *ts, char *id) {
    for (int i = ts->tamanho - 1; i >= 0; i--) {
        if (ts->simbolos[i].nivel_lexico < ts->escopo_atual) break; // Saiu do escopo atual
        if (strcmp(ts->simbolos[i].id, id) == 0) {
            return &ts->simbolos[i];
        }
    }
    return NULL;
}

void insere_simbolo(TabelaSimbolos *ts, SIMBOLO s) {
    if (ts->tamanho >= TAM_TABELA) {
        erro_semantico("Tabela de simbolos cheia!", contLinha);
        exit(1);
    }

    // Verifica se já existe um símbolo com mesmo nome NO ESCOPO ATUAL
    if (busca_escopo_atual(ts, s.id) != NULL) {
        char msg[200];
        sprintf(msg, "Redeclaracao do identificador '%s'.", s.id);
        erro_semantico(msg, contLinha);
        return; // Não insere
    }

    s.nivel_lexico = ts->escopo_atual;
    ts->simbolos[ts->tamanho++] = s;
}

SIMBOLO* busca_simbolo(TabelaSimbolos *ts, char *id) {
    // Procura do escopo mais interno para o mais externo
    for (int i = ts->tamanho - 1; i >= 0; i--) {
        if (strcmp(ts->simbolos[i].id, id) == 0) {
            return &ts->simbolos[i];
        }
    }
    return NULL; // Não encontrou
}

void incrementa_num_param(TabelaSimbolos *ts, char *id_func) {
    SIMBOLO* func = busca_simbolo(ts, id_func);
    if (func && func->categoria == CAT_FUNC) {
        func->info.func.num_parametros++;
    }
}

void set_tipo_param(TabelaSimbolos *ts, char *id_func, int tipo_param) {
    SIMBOLO* func = busca_simbolo(ts, id_func);
    if (func && func->categoria == CAT_FUNC) {
        int index = func->info.func.num_parametros;
        if (index < 10) { // Limite de parâmetros
            func->info.func.tipos_parametros[index] = tipo_param;
        }
    }
}


void imprime_tabela(TabelaSimbolos ts) {
    printf("\n--- Tabela de Simbolos ---\n");
    printf("ID\t\tCAT\tTIPO\tNIVEL\n");
    printf("------------------------------------\n");
    for (int i = 0; i < ts.tamanho; i++) {
        printf("%-15s\t%d\t%d\t%d\n",
               ts.simbolos[i].id,
               ts.simbolos[i].categoria,
               ts.simbolos[i].tipo,
               ts.simbolos[i].nivel_lexico);
    }
    printf("------------------------------------\n\n");
}