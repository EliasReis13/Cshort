#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include <stdbool.h>
#include "../lex/anaLex.h"

#define TAM_TABELA 100

typedef enum { CAT_VAR, CAT_FUNC, CAT_PARAM, CAT_PROT } Categoria;

typedef struct {
    int num_parametros;
    int tipos_parametros[10]; 
} FuncInfo;

typedef struct {
    char id[100];
    Categoria categoria;
    int tipo; // Usando os códigos PR_ de anaLex.h
    int nivel_lexico;
    int deslocamento;
    union { FuncInfo func; } info;
} SIMBOLO;

typedef struct {
    SIMBOLO simbolos[TAM_TABELA];
    int tamanho;
    int escopo_atual;
} TabelaSimbolos;

// --- Interface Pública ---
TabelaSimbolos inicializa_tabela();
void abre_escopo(TabelaSimbolos *ts);
void fecha_escopo(TabelaSimbolos *ts);
bool insere_simbolo(TabelaSimbolos *ts, SIMBOLO s); // Retorna bool
SIMBOLO* busca_simbolo(TabelaSimbolos *ts, char *id);
void imprime_tabela(TabelaSimbolos ts);
void incrementa_num_param(TabelaSimbolos *ts, char *id_func);
void set_tipo_param(TabelaSimbolos *ts, char *id_func, int tipo_param);

#endif