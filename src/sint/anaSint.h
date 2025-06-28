#ifndef ANASINT_H
#define ANASINT_H

#include <stdio.h>
#include "anaLex.h" // Analisador léxico do Cshort

extern FILE *fd;
extern TOKEN t;
extern char TABS[200];

void Prog();

void Decl();
void DeclVar();
int Tipo();
void Tipo_param();

void Func();

void Atrib();
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();

void Termo();
void Fator();
void Expres();
void Expres_simp();
#endif