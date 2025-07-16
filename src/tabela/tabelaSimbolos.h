#ifndef _TABELA_SIMBOLOS_
#define _TABELA_SIMBOLOS_

#include <stdbool.h>
#include <stdio.h>
#include "../lex/anaLex.h"

// --- Enums ---
typedef enum { GLOBAL, LOCAL } ESCOPO;
typedef enum { INT_, REAL_, CHAR_, BOOL_, NA_TIPO } TIPO;
typedef enum { VAR_GLOBAL, VAR_LOCAL, PROC, PROC_PAR, PROT_ } IDCATEGORIA;
typedef enum { VIVO, ZUMBI_ } ZUMBI;

// --- Structs ---
typedef struct tokenInfo {
    char lexema[31];
    ESCOPO escopo;
    TIPO tipo;
    IDCATEGORIA idcategoria;
    ZUMBI zumbi;
} TokenInfo;

typedef struct tabela {
    int topo;
    TokenInfo tokensTab[1024];
} Tabela;

// --- Interface Pública ---
extern Tabela tabela;

void printarTabela(int pos);
void limparTabela();
void resetTokenInfo(TokenInfo *token);
void retirarLocais();
void matarZumbis(int procPos);

// Protótipos refatorados
bool inserirNaTabela(TokenInfo token);
bool buscaDeclRep(TokenInfo token);
TokenInfo* buscaDecl(char *lexema);

#endif