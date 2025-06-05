#ifndef ANALEX 
#define ANALEX

#include <stdio.h>  // Biblioteca padrão de entrada e saída

// Tamanho máximo para o lexema de um token
#define TAM_MAX_LEXEMA 50
// Tamanho máximo para números
#define TAM_NUM 50

// Contador global de linhas do arquivo fonte
extern int contLinha; // Contador de linhas

// Enumeração das categorias de tokens reconhecidos pelo analisador léxico
enum TOKEN_CAT {
    ID = 1,           // Identificador
    CT_INT,           // Constante inteira
    CT_REAL,          // Constante real (float/double)
    CT_CHAR,          // Constante caractere
    CT_STRING,        // Constante string
    SN,               // Sinal (operador ou pontuação)
    COMENT,           // Comentário
    END_EXPRESSION,   // Fim de expressão (ex: ponto e vírgula)
    END_FILE,         // Fim de arquivo
    RESERVED_WORD     // Palavra reservada
};

// Enumeração dos sinais (operadores e pontuações) reconhecidos
enum SINAIS {
    OP_SOMA = 1,           // '+'
    OP_SUBTRACAO,          // '-'
    OP_MULTIPLICACAO,      // '*'
    OP_DIVISAO,            // '/'
    OP_ATRIBUICAO,         // '='
    OP_MAIOR,              // '>'
    OP_MAIOR_IGUAL,        // '>='
    OP_MENOR,              // '<'
    OP_MENOR_IGUAL,        // '<='
    OP_IGUAL,              // '=='
    OP_DIFERENTE,          // '!='
    OP_AND,                // '&&'
    OP_OR,                 // '||'
    ABRE_PARENTESES,       // '('
    FECHA_PARENTESES,      // ')'
    ABRE_COLCHETE,         // '['
    FECHA_COLCHETE,        // ']'
    ABRE_CHAVE,            // '{'
    FECHA_CHAVE,           // '}'
    VIRGULA                // ','
};

// Enumeração das palavras reservadas da linguagem
enum PALAVRA_RESERVADA {
    PR_IF = 1,     // if
    PR_ELSE,       // else
    PR_INT,        // int
    PR_FLOAT,      // float
    PR_CHAR,       // char
    PR_STRING,     // string
    PR_WHILE,      // while
    PR_FOR,        // for
    PR_RETURN,     // return
    PR_BREAK,      // break
    PR_CONTINUE    // continue
};

// Estrutura que representa um token identificado pelo analisador léxico
typedef 
    struct {
        enum TOKEN_CAT cat; // Categoria do token
        union {
            int codigo;                         // Código do token (pode ser usado para sinais ou palavras reservadas)
            char lexema[TAM_MAX_LEXEMA];        // Lexema do token (para identificadores, strings, etc)
            int int_value;                      // Valor inteiro (para constantes inteiras)
            double real_value;                  // Valor real (para constantes reais)
        };
        
    } TOKEN;

// Função principal do analisador léxico: recebe um arquivo e retorna o próximo token encontrado
TOKEN AnaLex(FILE *fd);

#endif