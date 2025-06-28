#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include "anaLex.h" // Inclui as definições do analisador léxico (TOKEN, etc.)

// --- Variáveis Globais Externas ---

extern FILE *fd;        // Ponteiro para o arquivo de código-fonte que está sendo analisado.
extern TOKEN t;         // Armazena o token atual que está sendo processado pelo parser.
extern char TABS[200];  // String usada para controlar a indentação na impressão da árvore sintática.

// --- Protótipos das Funções do Parser ---

//Função principal e ponto de entrada do analisador sintático.
void Prog();

// --- Funções de Análise de Declarações ---
void Decl();
void DeclVar(); 
int Tipo();
void Tipo_param(); 

// --- Funções de Análise de Estruturas e Comandos ---
void Func();

void Atrib();
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();


// --- Funções de Análise de Expressões ---
void Termo();
void Fator();
void Expres();
void Expres_simp();

#endif