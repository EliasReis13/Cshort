#ifndef TABELASIMBOLOS_H
#define TABELASIMBOLOS_H

#include <stdbool.h>
#include "../lex/anaLex.h"

#define TAM_TABELA 100

//================================================================================
// Definições de Tipos para Análise Semântica (Atividade 3)
//================================================================================

/**
 * @brief Enumeração dos tipos de dados semânticos da linguagem Cshort.
 *
 * As funções de análise de expressão 
 * retornarão um valor deste tipo.
 */

typedef enum {
    INT_,     // Inteiro
    REAL_,    // Ponto Flutuante
    CHAR_,    // Caractere
    BOOL_,    // Booleano 
    VOID_,    // Vazio
    NA_TIPO   // Representa um tipo de erro ou não aplicável
} TIPO;

typedef enum { CAT_VAR, CAT_FUNC, CAT_PARAM, CAT_PROT } Categoria;
typedef enum { PASS_POR_VALOR, PASS_POR_REFERENCIA } Passagem;

typedef struct {
    int num_parametros;
    TIPO tipos_parametros[10];
    TIPO tipo;
} FuncInfo;

typedef struct {
    char id[100];
    Categoria categoria;
    TIPO tipo;
    int nivel_lexico;
    bool is_array;
    int array_size;
    Passagem passagem;
    union { FuncInfo func; } info;
} SIMBOLO;

typedef struct {
    SIMBOLO simbolos[TAM_TABELA];
    int tamanho;
    int escopo_atual;
} TabelaSimbolos;

// Interface Pública
TabelaSimbolos inicializa_tabela();
void abre_escopo(TabelaSimbolos *ts);
void fecha_escopo(TabelaSimbolos *ts);
bool insere_simbolo(TabelaSimbolos *ts, SIMBOLO s);
SIMBOLO* busca_simbolo(TabelaSimbolos *ts, char *id);
void imprime_tabela(TabelaSimbolos ts);
void incrementa_num_param(TabelaSimbolos *ts, char *id_func);
void set_tipo_param(TabelaSimbolos *ts, char *id_func, TIPO tipo_param);
char* geraRotulo();


#endif