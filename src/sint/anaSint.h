/**
 * @file anaSint.h
 * @brief Arquivo de cabeçalho (Interface) do Analisador Sintático.
 */

#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include <stdbool.h>
#include "../lex/anaLex.h"
#include "../tabela/tabelaSimbolos.h"
#include "../gerador/geradorCodigo.h"

/**
 * @enum DECL_SINALIZADOR
 * @brief Sinaliza o tipo de declaração encontrada pela função Decl().
 * Usado para a estratégia de análise fatorada que evita backtracking.
 */
typedef enum {
    NO_DECL,      // Nenhuma declaração válida encontrada
    DECL_VAR,     // Declaração de variável(is)
    DECL_PROT,    // Declaração de protótipo de função
    DECL_FUNC     // Declaração e definição de função
} DECL_SINALIZADOR;

// --- Variáveis Globais Externas ---
extern FILE *fd;
extern TOKEN t, tLookahead;
extern bool houveErroSintatico;

// Ponto de entrada e rotinas de alto nível
void Prog();
DECL_SINALIZADOR Decl();
void corpo_func();
int  Tipo();

// Análise de parâmetros
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
void Cmd_atrib();

/**
 * @brief Protótipos para as funções de análise de expressão.
 * @note Todas retornam um valor do tipo TIPO. Esta é a base da
 * Definição Dirigida pela Sintaxe (DDS) para a checagem de tipos.
 */

TIPO Expr();
TIPO Expr_atrib();
TIPO Expr_ou();
TIPO Expr_e();
TIPO Expr_relacional();
TIPO Expr_aditiva();
TIPO Expr_multiplicativa();
TIPO Fator();

#endif