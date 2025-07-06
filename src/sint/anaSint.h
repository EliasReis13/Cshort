#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include <stdbool.h> 
#include "../lex/anaLex.h"

// --- Enum para o sinalizador de declaração ---
// Adicionado conforme especificado no PDF para a estratégia de análise fatorada.
typedef enum { 
    NO_DECL, 
    DECL_VAR, 
    DECL_PROT, 
    DECL_FUNC 
} DECL_SINALIZADOR;

// --- Variáveis Globais Externas ---
extern FILE *fd;
extern TOKEN t;
extern TOKEN tLookahead; // Adicionado para suportar a análise com lookahead
extern char TABS[200];
extern int contLinha;
extern bool modoPanico;
extern bool houveErroSintatico;  

// --- Protótipos das Funções do Parser ---

// Ponto de entrada e rotinas de alto nível (estrutura do PDF)
void Prog();
DECL_SINALIZADOR Decl(); // Modificada para retornar o sinalizador [cite: 39]
void corpo_func();       // Nova função para tratar o corpo de uma função [cite: 32]
int  Tipo();

// Análise de parâmetros
void Tipos_param();

// Análise de Comandos (permanecem os mesmos)
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();
void Cmd_bloco();
void Cmd_break();
void Cmd_continue();

// Análise de Expressões (permanecem as mesmas)
void Expr();
void Expr_atrib();
void Expr_ou();
void Expr_e();
void Expr_relacional();
void Expr_aditiva();
void Expr_multiplicativa();
void Fator();

#endif