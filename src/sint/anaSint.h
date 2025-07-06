#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include <stdbool.h> // Adicionado para bool
#include "../lex/anaLex.h"

// --- Variáveis Globais Externas ---
extern FILE *fd;
extern TOKEN t;
extern char TABS[200];
extern int contLinha;
extern bool modoPanico;
extern bool houveErroSintatico;  

// --- Protótipos das Funções do Parser ---

// Ponto de entrada e rotinas de alto nível
void Prog();
void Decl_ou_Func();
int  Tipo();

// Análise de corpos de função e declaração
void Func_body();
void Decl_var_body();
void Tipos_param();

// Análise de Comandos
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();
void Cmd_bloco();
void Cmd_break();
void Cmd_continue();

// Análise de Expressões (com tratamento de precedência)
void Expr();
void Expr_atrib();
void Expr_ou();
void Expr_e();
void Expr_relacional();
void Expr_aditiva();
void Expr_multiplicativa();
void Fator();

#endif