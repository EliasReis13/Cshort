#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabelaSimbolos.h"

extern int contLinha;

TabelaSimbolos inicializa_tabela() {
    TabelaSimbolos ts;
    ts.tamanho = 0;
    ts.escopo_atual = 0; 
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

bool insere_simbolo(TabelaSimbolos *ts, SIMBOLO s) {
    if (ts->tamanho >= TAM_TABELA) {
        // erro_semantico("Tabela de simbolos cheia!", contLinha); // Será tratado no anaSint
        return false;
    }
    if (busca_escopo_atual(ts, s.id) != NULL) {
        return false; // Falha: Símbolo já existe
    }
    s.nivel_lexico = ts->escopo_atual;
    ts->simbolos[ts->tamanho++] = s;
    return true; // Sucesso
}

SIMBOLO* busca_simbolo(TabelaSimbolos *ts, char *id) {
    for (int i = ts->tamanho - 1; i >= 0; i--) {
        if (strcmp(ts->simbolos[i].id, id) == 0) {
            return &ts->simbolos[i];
        }
    }
    return NULL;
}
void incrementa_num_param(TabelaSimbolos *ts, char *id_func) {
    SIMBOLO* func = busca_simbolo(ts, id_func);
    if (func && (func->categoria == CAT_FUNC || func->categoria == CAT_PROT)) {
        func->info.func.num_parametros++;
    }
}

void set_tipo_param(TabelaSimbolos *ts, char *id_func, TIPO tipo_param) {
    SIMBOLO* func = busca_simbolo(ts, id_func);
    if (func && (func->categoria == CAT_FUNC || func->categoria == CAT_PROT)) {
        int index = func->info.func.num_parametros;
        if (index < 10) {
            func->info.func.tipos_parametros[index] = tipo_param;
        }
    }
}

// Função para gerar rótulos >>>
char* geraRotulo() {
    static int contador_rotulo = 0;
    static char buffer_rotulo[20];
    sprintf(buffer_rotulo, "L%d", contador_rotulo++);
    return buffer_rotulo;
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
const char* get_tipo_string(TIPO tipo) {
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
    printf("%-20s | %-6s | %-12s | %-10s\n", "LEXEMA", "ESCOPO", "CLASSE", "TIPO");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < ts.tamanho; i++) {
        // Busca as strings correspondentes para classe e tipo
        const char* classe_str = get_classe_string(ts.simbolos[i].categoria);
        const char* tipo_str = get_tipo_string(ts.simbolos[i].tipo);

        printf("%-20s | %-6d | %-12s | %-10s\n",
               ts.simbolos[i].id,           // LEXEMA
               ts.simbolos[i].nivel_lexico, // ESCOPO
               classe_str,                  // CLASSE
               tipo_str                     // TIPO
        );
    }
    printf("-----------------------------------------------------------\n\n");
}
