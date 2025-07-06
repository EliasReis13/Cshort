// Arquivo: src/tabela/tabelaSimbolos.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabelaSimbolos.h"

// É necessário ter acesso a estas variáveis globais para registrar erros.
// Certifique-se de que elas estejam declaradas como 'extern' em tabelaSimbolos.h ou aqui.
extern int contLinha;
extern bool houveErroSemantico;

// Definição da flag de erro semântico
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
        // Se já passamos do escopo atual na busca reversa, podemos parar
        if (ts->simbolos[i].nivel_lexico < ts->escopo_atual) {
            break;
        }
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
        return; // Não insere para evitar duplicatas no mesmo escopo
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
    if (func && (func->categoria == CAT_FUNC || func->categoria == CAT_PROT)) {
        func->info.func.num_parametros++;
    }
}

void set_tipo_param(TabelaSimbolos *ts, char *id_func, int tipo_param) {
    SIMBOLO* func = busca_simbolo(ts, id_func);
    if (func && (func->categoria == CAT_FUNC || func->categoria == CAT_PROT)) {
        int index = func->info.func.num_parametros;
        if (index < 10) { // Limite de parâmetros (definido em FuncInfo)
            func->info.func.tipos_parametros[index] = tipo_param;
        }
    }
}

// Função para converter o código da categoria em uma string legível
const char* get_classe_string(Categoria cat) {
    switch (cat) {
        case CAT_VAR:   return "Variavel";
        case CAT_FUNC:  return "Funcao";
        case CAT_PARAM: return "Parametro";
        case CAT_PROT:  return "Prototipo";
        default:        return "Desconhecida";
    }
}

// Função para converter o código do tipo em uma string legível
// Os valores (PR_INT, etc.) vêm do seu anaLex.h
const char* get_tipo_string(int tipo) {
    switch (tipo) {
        case PR_INT:    return "Inteiro";
        case PR_FLOAT:  return "Float";
        case PR_CHAR:   return "Char";
        case PR_STRING: return "String";
        case PR_VOID:   return "Void";
        default:        return "N/A";
    }
}

void imprime_tabela(TabelaSimbolos ts) {
    printf("\n--- Tabela de Simbolos ---\n");
    // Novo cabeçalho na ordem solicitada: lexema, escopo, classe, tipo
    printf("%-20s | %-6s | %-12s | %-10s\n", "LEXEMA", "ESCOPO", "CLASSE", "TIPO");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < ts.tamanho; i++) {
        // Busca as strings correspondentes para classe e tipo
        const char* classe_str = get_classe_string(ts.simbolos[i].categoria);
        const char* tipo_str = get_tipo_string(ts.simbolos[i].tipo);

        // Imprime na nova ordem com os valores de texto
        printf("%-20s | %-6d | %-12s | %-10s\n",
               ts.simbolos[i].id,           // LEXEMA
               ts.simbolos[i].nivel_lexico, // ESCOPO
               classe_str,                  // CLASSE
               tipo_str                     // TIPO
        );
    }
    printf("-----------------------------------------------------------\n\n");
}