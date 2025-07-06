#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "../lex/anaLex.h"

#define TAM_TABELA 100

// Categoria do símbolo
typedef enum {
    CAT_VAR,
    CAT_FUNC,
    CAT_PARAM,
    CAT_PROT // Protótipo de função
} Categoria;

// Estrutura para informações da função
typedef struct {
    int num_parametros;
    int tipos_parametros[10]; 
} FuncInfo;

// Estrutura de um símbolo na tabela
typedef struct {
    char id[100];       // Nome do identificador
    Categoria categoria;  // var, func, param...
    int tipo;           // int, float, char (usando os códigos de PR_ da anaLex)
    int nivel_lexico;   // Nível do escopo onde foi declarado
    int deslocamento;   // Deslocamento de memória (para geração de código)

    union {
        FuncInfo func;
    } info;

} SIMBOLO;

// Estrutura da Tabela de Símbolos
typedef struct {
    SIMBOLO simbolos[TAM_TABELA];
    int tamanho;
    int escopo_atual;
} TabelaSimbolos;

// --- Funções Públicas ---

TabelaSimbolos inicializa_tabela();
void abre_escopo(TabelaSimbolos *ts);
void fecha_escopo(TabelaSimbolos *ts);
void insere_simbolo(TabelaSimbolos *ts, SIMBOLO s);
SIMBOLO* busca_simbolo(TabelaSimbolos *ts, char *id);
void imprime_tabela(TabelaSimbolos ts);
void erro_semantico(char* msg, int linha);

// Funções específicas para manipulação de funções
void incrementa_num_param(TabelaSimbolos *ts, char *id_func);
void set_tipo_param(TabelaSimbolos *ts, char *id_func, int tipo_param);

#endif