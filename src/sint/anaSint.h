/**
 * @file anaSint.h
 * @brief Arquivo de cabeçalho (Interface) do Analisador Sintático.
 *
 * @purpose
 * Este arquivo define a "interface pública" do módulo de análise sintática.
 * Ele contém as definições de tipos, declarações de variáveis globais externas
 * e protótipos de funções que são necessários para que outros módulos (como o main.c)
 * possam interagir com o analisador.
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
 *
 * Esta enumeração é usada como um mecanismo de comunicação entre a função Decl() e
 * seu chamador (Prog()). Ela informa se a declaração analisada foi de uma variável,
 * um protótipo de função ou uma definição completa de função, permitindo que o
 * parser saiba se deve ou não esperar um corpo de função `{...}` em seguida.
 */
typedef enum {
    NO_DECL,      // Nenhuma declaração válida encontrada
    DECL_VAR,     // Declaração de variável(is)
    DECL_PROT,    // Declaração de protótipo de função
    DECL_FUNC     // Declaração e definição de função
} DECL_SINALIZADOR;

// --- Variáveis Globais Externas ---
/**
 * A palavra-chave 'extern' indica que estas variáveis são definidas em outro
 * arquivo (neste caso, anaSint.c), mas podem ser acessadas por qualquer
 * arquivo que inclua este cabeçalho.
 */
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

// Análise de Expressões
TIPO Expr();
TIPO Expr_atrib();
TIPO Expr_ou();
TIPO Expr_e();
TIPO Expr_relacional();
TIPO Expr_aditiva();
TIPO Expr_multiplicativa();
TIPO Fator();

#endif