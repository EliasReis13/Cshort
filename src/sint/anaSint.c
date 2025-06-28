#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasint.h"
#include "analex.h"

// Protótipos das funções locais (regras da gramática)
void Prog();
void Decl();
void Decl_var();
int  Tipo();
void Tipos_param();
void Cmd();
void Decl_ou_Func();
void Func_body();
void Decl_var_body();
void Expr();
void Expr_atrib();
void Expr_ou();
void Expr_e();
void Expr_relacional();
void Expr_aditiva();
void Expr_multiplicativa();
void Fator();
void print_folha(TOKEN tk);
void aumenta_ident();
void diminui_ident();

// --- Funções Utilitárias ---

//Adiciona espaços à string de indentação 'TABS' para aprofundar um nível na árvore.
void aumenta_ident(){
    if (strlen(TABS) < sizeof(TABS) - 3){
        strcat(TABS, "  "); // Adiciona dois espaços para cada nível
    }
}

//Remove espaços da string de indentação 'TABS' para retornar um nível na árvore.
void diminui_ident() {
    if (strlen(TABS) >= 2) {
        TABS[strlen(TABS) - 2] = '\0';
    }
}

/**
 * @brief Imprime um nó folha da árvore de análise, que representa um token terminal.
 * @param tk O token a ser impresso.
 */
void print_folha(TOKEN tk) {
    printf("%s- ", TABS); // O traço indica que é um nó folha
    switch (tk.cat) {
        case ID: printf("ID: %s\n", tk.lexema); break;
        case SN: printf("SN: %d\n", tk.codigo); break;
        case CT_INT: printf("CT_INT: %d\n", tk.int_value); break;
        case CT_REAL: printf("CT_REAL: %f\n", tk.real_value); break;
        case RESERVED_WORD: printf("PR: %d\n", tk.codigo); break;
        default: printf("TOKEN (cat %d)\n", tk.cat); break;
    }
}

/**
 * @brief Função "match". Verifica se o token atual é o esperado pela gramática.
 * Se for, avança para o próximo token. Se não for, emite um erro sintático.
 * @param categoria_esperada A categoria de token esperada (e.g., SN, ID).
 * @param codigo_esperado O código específico do token (e.g., ABRE_PARENTESES), se aplicável.
 */
void consome(int categoria_esperada, int codigo_esperado) {
    if (t.cat == categoria_esperada && (codigo_esperado == 0 || t.codigo == codigo_esperado)) {
        // Avança para o próximo token se a verificação for bem-sucedida
        t = AnaLex(fd);
    } else {
        // Dispara um erro e termina a execução se o token não for o esperado.
        char msg_erro[200];
        sprintf(msg_erro, "Erro sintatico na linha %d: esperado (cat %d, cod %d), mas encontrei (cat %d, cod %d)",
                contLinha, categoria_esperada, codigo_esperado, t.cat, t.codigo);
        error(msg_erro);
    }
}


// --- Funções de Análise Gramatical ---

/**
 * @brief Verifica se o token atual é uma palavra reservada que define um tipo.
 * @return 1 se for um tipo (int, float, etc.), 0 caso contrário.
 */
int Tipo() {
    if (t.cat == RESERVED_WORD) {
        switch (t.codigo) {
            case PR_INT:
            case PR_CHAR:
            case PR_STRING:
            case PR_FLOAT:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

/**
 * @brief Ponto de entrada e regra inicial do parser.
 * Gramática: Prog -> (Decl_ou_Func)* END_FILE
 */
void Prog() {
    printf("%s<Prog>\n", TABS); aumenta_ident();
    t = AnaLex(fd); // Lê o primeiro token do arquivo
    while (t.cat != END_FILE) {
        if (Tipo()) {
            Decl_ou_Func();
        } else {
            error("Erro sintatico: esperado uma declaracao ou uma funcao.");
        }
    }
    diminui_ident(); printf("%s</Prog>\n", TABS);
}

/**
 * @brief Decide se a construção é uma declaração de variável ou de função.
 * A decisão é baseada no token que segue o identificador (um '(' indica função).
 * Gramática: Decl_ou_Func -> Tipo ID (Func_body | Decl_var_body)
 */
void Decl_ou_Func() {
    printf("%s<Decl_ou_Func>\n", TABS); aumenta_ident();
    print_folha(t); consome(t.cat, t.codigo); // Consome o tipo (e.g., 'int')
    print_folha(t); consome(ID, 0);          // Consome o identificador (e.g., 'main')

    if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        Func_body(); // É uma função
    } else {
        Decl_var_body(); // É uma declaração de variável(is)
    }
    diminui_ident(); printf("%s</Decl_ou_Func>\n", TABS);
}

/**
 * @brief Analisa o corpo de uma função.
 * Gramática: Func_body -> '(' Tipos_param ')' '{' Decl* Cmd* '}'
 */
void Func_body() {
    printf("%s<Func_body>\n", TABS); aumenta_ident();
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Tipos_param();
    print_folha(t); consome(SN, FECHA_PARENTESES);
    print_folha(t); consome(SN, ABRE_CHAVE);
    // Analisa declarações de variáveis locais no início da função
    while (Tipo()) {
        Decl();
    }
    // Analisa os comandos da função
    while (!(t.cat == SN && t.codigo == FECHA_CHAVE)) {
        Cmd();
    }
    print_folha(t); consome(SN, FECHA_CHAVE);
    diminui_ident(); printf("%s</Func_body>\n", TABS);
}

/**
 * @brief Analisa o resto de uma linha de declaração de variável global.
 */
void Decl_var_body() {
    printf("%s<Decl_var_body>\n", TABS); aumenta_ident();
    if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
        print_folha(t); consome(SN, ABRE_COLCHETE);
        print_folha(t); consome(CT_INT, 0);
        print_folha(t); consome(SN, FECHA_COLCHETE);
    }
    while (t.cat == SN && t.codigo == VIRGULA) {
        print_folha(t); consome(SN, VIRGULA);
        Decl_var();
    }
    // No C, declarações terminam com ';' (END_EXPRESSION)
    print_folha(t); consome(END_EXPRESSION, 0);
    diminui_ident(); printf("%s</Decl_var_body>\n", TABS);
}

/**
 * @brief Analisa uma linha de declaração de variável local.
 * Gramática: Decl -> Tipo ID (',' ID)* ';'
 */
void Decl() {
    printf("%s<Decl>\n", TABS); aumenta_ident();
    if (Tipo()) {
        Decl_var(); 
        while (t.cat == SN && t.codigo == VIRGULA) {
            print_folha(t); consome(SN, VIRGULA);
            print_folha(t); consome(ID, 0);
        }
        print_folha(t); consome(END_EXPRESSION, 0); // Consome o ';'
    } else {
        error("Declaracao de variavel local esperada.");
    }
    diminui_ident(); printf("%s</Decl>\n", TABS);
}

/**
 * @brief Analisa uma única declaração de variável (tipo + id + opcionalmente vetor).
 * Gramática: Decl_var -> Tipo ID ('[' CT_INT ']')?
 */
void Decl_var() {
    printf("%s<Decl_var>\n", TABS); aumenta_ident();
    print_folha(t); consome(t.cat, t.codigo); 
    print_folha(t); consome(ID, 0);

    if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
        print_folha(t); consome(SN, ABRE_COLCHETE);
        print_folha(t); consome(CT_INT, 0);
        print_folha(t); consome(SN, FECHA_COLCHETE);
    }
    diminui_ident(); printf("%s</Decl_var>\n", TABS);
}

/**
 * @brief Analisa a lista de parâmetros de uma função.
 * Gramática: Tipos_param -> (Tipo ID ('[]')? (',' Tipo ID ('[]')?)* )?
 */
void Tipos_param() {
    printf("%s<Tipos_param>\n", TABS); aumenta_ident();
    // Verifica se a lista de parâmetros não está vazia.
    // Se o próximo token não for ')', então deve haver parâmetros.
    if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
        if (Tipo()) {
            print_folha(t); consome(t.cat, t.codigo);
            print_folha(t); consome(ID, 0);
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                print_folha(t); consome(SN, ABRE_COLCHETE);
                print_folha(t); consome(SN, FECHA_COLCHETE);
            }
            while (t.cat == SN && t.codigo == VIRGULA) {
                print_folha(t); consome(SN, VIRGULA);
                if (Tipo()) {
                    print_folha(t); consome(t.cat, t.codigo);
                    print_folha(t); consome(ID, 0);
                    if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                        print_folha(t); consome(SN, ABRE_COLCHETE);
                        print_folha(t); consome(SN, FECHA_COLCHETE);
                    }
                } else {
                    error("Esperado um tipo no parametro da funcao.");
                }
            }
        } else {
            error("Parametro de funcao invalido. Esperado um tipo ou ')'");
        }
    }
    diminui_ident(); printf("%s</Tipos_param>\n", TABS);
}

/**
 * @brief Analisa um comando da linguagem.
 * Gramática: Cmd -> if | while | return | Bloco | Expr ';'
 */
void Cmd() {
    printf("%s<Cmd>\n", TABS); aumenta_ident();
    if (t.cat == RESERVED_WORD && t.codigo == PR_IF) {
        // Lógica para o comando 'if-else'
        print_folha(t); consome(RESERVED_WORD, PR_IF);
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
        Cmd();
        if (t.cat == RESERVED_WORD && t.codigo == PR_ELSE) {
            print_folha(t); consome(RESERVED_WORD, PR_ELSE);
            Cmd();
        }
    } else if (t.cat == RESERVED_WORD && t.codigo == PR_WHILE) {
        // Lógica para o comando 'while'
        print_folha(t); consome(RESERVED_WORD, PR_WHILE);
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
        Cmd();
    } else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) {
        // Lógica para o comando 'return'
        print_folha(t); consome(RESERVED_WORD, PR_RETURN);
        if (t.cat != END_EXPRESSION) {
            Expr();
        }
        print_folha(t); consome(END_EXPRESSION, 0);
    } else if (t.cat == SN && t.codigo == ABRE_CHAVE) {
        // Lógica para um bloco de comandos '{ ... }'
        print_folha(t); consome(SN, ABRE_CHAVE);
        while (!(t.cat == SN && t.codigo == FECHA_CHAVE)) {
            Cmd();
        }
        print_folha(t); consome(SN, FECHA_CHAVE);
    } else {
        Expr();
        print_folha(t); consome(END_EXPRESSION, 0);
    }
    diminui_ident(); printf("%s</Cmd>\n", TABS);
}

// --- Funções de Análise de Expressão (com Precedência de Operadores) ---

void Expr() {
    printf("%s<Expr>\n", TABS); aumenta_ident();
    Expr_atrib();
    diminui_ident(); printf("%s</Expr>\n", TABS);
}

void Expr_atrib() {
    printf("%s<Expr_atrib>\n", TABS); aumenta_ident();
    Expr_ou();
    // Trata atribuição, que tem associatividade à direita
    if (t.cat == SN && t.codigo == OP_ATRIBUICAO) {
        print_folha(t); consome(SN, OP_ATRIBUICAO);
        Expr_atrib();
    }
    diminui_ident(); printf("%s</Expr_atrib>\n", TABS);
}

void Expr_ou() {
    printf("%s<Expr_ou>\n", TABS); aumenta_ident();
    Expr_e();
    // Trata o operador '||' (associatividade à esquerda)
    while (t.cat == SN && t.codigo == OP_OR) {
        print_folha(t); consome(SN, OP_OR);
        Expr_e();
    }
    diminui_ident(); printf("%s</Expr_ou>\n", TABS);
}

void Expr_e() {
    printf("%s<Expr_e>\n", TABS); aumenta_ident();
    Expr_relacional();
    // Trata o operador '&&' (associatividade à esquerda)
    while (t.cat == SN && t.codigo == OP_AND) {
        print_folha(t); consome(SN, OP_AND);
        Expr_relacional();
    }
    diminui_ident(); printf("%s</Expr_e>\n", TABS);
}

void Expr_relacional() {
    printf("%s<Expr_relacional>\n", TABS); aumenta_ident();
    Expr_aditiva();
    // Trata operadores relacionais (==, !=, >, <, etc.)
    while (t.cat == SN && (t.codigo == OP_IGUAL || t.codigo == OP_DIFERENTE || t.codigo == OP_MAIOR || t.codigo == OP_MENOR || t.codigo == OP_MAIOR_IGUAL || t.codigo == OP_MENOR_IGUAL)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_aditiva();
    }
    diminui_ident(); printf("%s</Expr_relacional>\n", TABS);
}

void Expr_aditiva() {
    printf("%s<Expr_aditiva>\n", TABS); aumenta_ident();
    Expr_multiplicativa();
    // Trata operadores '+' e '-' (associatividade à esquerda)
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_multiplicativa();
    }
    diminui_ident(); printf("%s</Expr_aditiva>\n", TABS);
}

void Expr_multiplicativa() {
    printf("%s<Expr_multiplicativa>\n", TABS); aumenta_ident();
    Fator();
    // Trata operadores '*' e '/' (associatividade à esquerda)
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        print_folha(t); consome(SN, t.codigo);
        Fator();
    }
    diminui_ident(); printf("%s</Expr_multiplicativa>\n", TABS);
}

/**
 * @brief Analisa a menor unidade de uma expressão (um 'fator').
 * Gramática: Fator -> Unário Fator | ID Acesso? | Constante | '(' Expr ')'
 */
void Fator() {
    printf("%s<Fator>\n", TABS); aumenta_ident();
    // Trata operadores unários (+, -, !)
    if (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO || t.codigo == OP_NOT)) {
        print_folha(t); consome(SN, t.codigo);
        Fator();
    } else if (t.cat == ID) {
        print_folha(t); consome(ID, 0);
        // Verifica se é um acesso a vetor
        if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
            print_folha(t); consome(SN, ABRE_COLCHETE);
            Expr();
            print_folha(t); consome(SN, FECHA_COLCHETE);
        // Verifica se é uma chamada de função
        } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
            print_folha(t); consome(SN, ABRE_PARENTESES);
            if (!(t.cat == SN && t.codigo == FECHA_PARENTESES)) {
                Expr();
                while (t.cat == SN && t.codigo == VIRGULA) {
                    print_folha(t); consome(SN, VIRGULA);
                    Expr();
                }
            }
            print_folha(t); consome(SN, FECHA_PARENTESES);
        }
    // Trata constantes
    } else if (t.cat == CT_INT || t.cat == CT_REAL || t.cat == CT_CHAR) {
        print_folha(t); consome(t.cat, 0);
    // Trata expressões entre parênteses
    } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
    } else {
        error("Fator mal formado em uma expressao (esperado ID, constante ou '(' ).");
    }
    diminui_ident(); printf("%s</Fator>\n", TABS);
}