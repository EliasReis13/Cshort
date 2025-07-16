/**
 * @file anaSint.c
 * @brief Implementação do Analisador Sintático e Semântico.
 *
 * @purpose
 * Este arquivo contém a lógica completa do analisador de descida recursiva.
 * Ele implementa as regras da gramática da linguagem Cshort e integra as
 * ações semânticas, como o gerenciamento de escopo e a verificação de
 * declarações, através da interação com o módulo da Tabela de Símbolos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "anaSint.h"
#include "../lex/anaLex.h"
#include "../tabela/tabelaSimbolos.h"

// --- Variáveis Globais ---
FILE *fd;
TOKEN t, tLookahead;
char TABS[200] = "";
TabelaSimbolos ts; 

// --- Variáveis para Análise Semântica ---
TokenInfo current_token_info;
ESCOPO current_scope = GLOBAL;

// --- Protótipos de Funções Auxiliares Locais ---
void nextToken();
void error(char msg[]);
void aumenta_ident();
void diminui_ident();
void print_folha(TOKEN tk);
void 

// Protótipos do Parser (alguns precisam de novos parâmetros)
void Prog();
DECL_SINALIZADOR Decl();
void corpo_func();
void Tipos_param(char* nome_funcao); 
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();
void Cmd_break();
void Cmd_continue();
void Cmd_bloco();
void Expr();
void Expr_atrib();
void Expr_ou();
void Expr_e();
void Expr_relacional();
void Expr_aditiva();
void Expr_multiplicativa();
void Fator();


// --- Implementação das Funções Auxiliares ---

TIPO token_to_tipo(int token_code) {
    switch (token_code) {
        case PR_INT: return INT_;
        case PR_FLOAT: return REAL_;
        case PR_CHAR: return CHAR_;
        default: return NA_TIPO;
    }
}

/**
 * @brief Reporta um erro sintático 
 * @param msg Mensagem de erro a ser exibida.
 */
void error(char msg[]) {
    printf("[ERRO | Linha %d]: %s\n", contLinha, msg);
    exit(1);
}

/**
 * @brief Adiciona espaços para indentar a saída da árvore de análise, melhorando a visualização.
 */
void aumenta_ident() {
    strcat(TABS, "  ");
}

/**
 * @brief Remove espaços da indentação ao sair de um nível da árvore de análise.
 */
void diminui_ident() {
    int len = strlen(TABS);
    if (len >= 2) TABS[len - 2] = '\0';
}

/**
 * @brief Imprime um nó folha da árvore de análise sintática.
 * @param tk O token a ser impresso.
 */

void print_folha(TOKEN tk) {
    printf("%s- ", TABS);
    switch (tk.cat) {
        case ID: printf("ID: %s\n", tk.lexema); break;
        case SN: printf("SN: %d\n", tk.codigo); break;
        case CT_INT: printf("CT_INT: %d\n", tk.int_value); break;
        case CT_REAL: printf("CT_REAL: %f\n", tk.real_value); break;
        case CT_CHAR: printf("CT_CHAR: %d\n", tk.int_value); break;
        case CT_STRING: printf("CT_STRING: %s\n", tk.lexema); break;
        case RESERVED_WORD: printf("PR: %d\n", tk.codigo); break;
        default: printf("TOKEN(cat=%d)\n", tk.cat);
    }
}

/**
 * @brief Avança para o próximo token do arquivo de entrada.
 */

void nextToken() {
    t = tLookahead;
    if (t.cat != END_FILE) {
        tLookahead = AnaLex(fd);
    }
}

/**
 * @brief Consome o token atual se ele corresponder à categoria e código esperados.
 * @param categoria Categoria do token esperado.
 * @param codigo Código do token esperado (relevante para SN e RESERVED_WORD).
 */

void consome(int categoria, int codigo) {

    if ((int)t.cat == categoria && (codigo == 0 || t.codigo == codigo)) {
        print_folha(t);
        nextToken();
    } else {
        char msg[200];
        sprintf(msg, "Esperado (cat=%d, cod=%d), encontrado token '%s' (cat=%d, cod=%d)",
                categoria, codigo, t.lexema, t.cat, t.codigo);
        error(msg);
    }
}

// --- Implementação das Funções do Parser com Semântica ---

/**
 * @brief Verifica se o token atual é um tipo válido (int, float, etc.).
 * @return int Retorna 1 se for um tipo, 0 caso contrário.
 */

int Tipo() {
    if (t.cat == RESERVED_WORD) {
        switch (t.codigo) {
            case PR_INT:
            case PR_FLOAT:
            case PR_CHAR:
            case PR_STRING:
            case PR_VOID:
                return 1;
        }
    }
    return 0;
}

/**
 * @brief Função inicial da gramática (símbolo de partida).
 * @purpose Orquestra a análise do programa inteiro.
 */

void Prog() {
    printf("<Prog>\n");
    aumenta_ident();

    ts = inicializa_tabela(); 

    tLookahead = AnaLex(fd);
    nextToken();

    while (t.cat != END_FILE) {
        DECL_SINALIZADOR flag = Decl();
        if (flag == DECL_FUNC) {
            corpo_func();
        }
    }

    imprime_tabela(ts); 
    diminui_ident();
    printf("</Prog>\n");
}

/**
 * @brief Analisa o corpo de uma função, que é um bloco de comandos.
 * @note O escopo da função (para parâmetros e variáveis locais) é gerenciado aqui.
 */

void corpo_func() {
    printf("%s<corpo_func>\n", TABS);
    aumenta_ident();
    
    abre_escopo(&ts);
    Cmd_bloco();
    fecha_escopo(&ts); 

    diminui_ident();
    printf("%s</corpo_func>\n", TABS);
}

/**
 * @brief Analisa uma declaração de variável, função ou protótipo.
 * @return DECL_SINALIZADOR - Informa o tipo de declaração encontrada.
 * @note Esta função usa lookahead para decidir o caminho da análise e realiza
 * a inserção de novos símbolos na tabela.
 */

DECL_SINALIZADOR Decl() {
    DECL_SINALIZADOR declFlag = NO_DECL;

    if (!Tipo()) {
        error("Esperado um tipo (int, float, char, etc) no inicio da declaracao.");
        nextToken();
        
        return NO_DECL;
    }

    int tipo_declarado = t.codigo;
    nextToken(); 
    if (t.cat != ID) {
        error("Identificador esperado apos o tipo.");
        
        return NO_DECL;
    }

    char nome_id[100];
    strcpy(nome_id, t.lexema); 

    if (tLookahead.cat == SN && tLookahead.codigo == ABRE_PARENTESES) {
        SIMBOLO s;
        strcpy(s.id, nome_id);
        s.tipo = tipo_declarado;
        s.categoria = CAT_FUNC;
        s.info.func.num_parametros = 0; // Inicializa contagem de parâmetros
        insere_simbolo(&ts, s);

        nextToken(); // Consome o ID da função
        nextToken(); // Consome o '('

        // Abre um escopo temporário para os parâmetros
        abre_escopo(&ts);
        Tipos_param(nome_id); 
        consome(SN, FECHA_PARENTESES);

        if (t.cat == SN && t.codigo == ABRE_CHAVE) {
            declFlag = DECL_FUNC; 
        } else {
            declFlag = DECL_PROT; 
            s.categoria = CAT_PROT; // Atualiza categoria para protótipo
            fecha_escopo(&ts); 
            consome(END_EXPRESSION, 0);
        }
    } else {
        
        declFlag = DECL_VAR;
        if (tipo_declarado == PR_VOID) {
            erro_semantico("Variaveis nao podem ser do tipo 'void'.", contLinha);
        }

        // Loop para tratar múltiplas declarações (ex: int a, b, c;)
        while(true) {
            if (t.cat != ID) {
                error("Identificador de variavel esperado.");
                break;
            }
            
            SIMBOLO s;
            strcpy(s.id, t.lexema);
            s.tipo = tipo_declarado;
            s.categoria = CAT_VAR;
            insere_simbolo(&ts, s);
            
            nextToken(); // Consome o ID da variável

            if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                nextToken();
                if (t.cat != CT_INT) error("Uma constante inteira era esperada para o tamanho do array.");
                nextToken();
                if (t.cat != SN || t.codigo != FECHA_COLCHETE) error("']' esperado para fechar a definicao do array.");
                nextToken();
            }

            if (t.cat == SN && t.codigo == VIRGULA) {
                nextToken(); // Consome a vírgula e continua o loop
            } else {
                break; // A lista de declarações terminou
            }
        }
        consome(END_EXPRESSION, 0); // Consome o ';' no final
    }
    return declFlag;
}

/**
 * @brief Analisa os parâmetros na declaração de uma função.
 * @param nome_funcao O nome da função à qual os parâmetros pertencem, para
 * registrar as informações na tabela de símbolos.
 */
void Tipos_param(char* nome_funcao) {
    if (t.cat == RESERVED_WORD && t.codigo == PR_VOID) {
        if (tLookahead.cat == SN && tLookahead.codigo == FECHA_PARENTESES) {
            consome(RESERVED_WORD, PR_VOID);
            return;
        }
    }

    if (Tipo()) {
        while (true) {
            if (!Tipo()) break;
            int tipo_param = t.codigo;
            consome(t.cat, t.codigo); // Consome o tipo

            // --- LÓGICA SEMÂNTICA PARA PARÂMETROS ---
            SIMBOLO p;
            p.tipo = tipo_param;
            p.categoria = CAT_PARAM;
            set_tipo_param(&ts, nome_funcao, tipo_param); // Informa o tipo para a função pai

            if (t.cat == ID) {
                strcpy(p.id, t.lexema);
                insere_simbolo(&ts, p); // Insere o parâmetro como uma variável no escopo
                incrementa_num_param(&ts, nome_funcao);
                consome(ID, 0);
            } else {
                // Parâmetro sem nome (comum em protótipos)
                strcpy(p.id, ""); // Sem nome
                // Não insere na tabela como símbolo buscável, mas conta
                incrementa_num_param(&ts, nome_funcao);
            }
            
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                consome(SN, ABRE_COLCHETE);
                consome(SN, FECHA_COLCHETE);
            }

            if (t.cat != SN || t.codigo != VIRGULA) {
                break;
            }
            consome(SN, VIRGULA);
        }
    }
}

/**
 * @brief Analisa um comando genérico.
 * @note Esta função atua como um distribuidor, chamando a rotina de análise
 * apropriada com base no token atual (if, while, {, etc.).
 */
void Cmd() {
    if (Tipo()) { 
        Decl();
    }
    else if (t.cat == RESERVED_WORD && t.codigo == PR_IF) Cmd_if();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_WHILE) Cmd_while();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_FOR) Cmd_for();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) Cmd_return();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_BREAK) Cmd_break();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_CONTINUE) Cmd_continue();
    else if (t.cat == SN && t.codigo == ABRE_CHAVE) {
        abre_escopo(&ts);
        Cmd_bloco();
        fecha_escopo(&ts); 
    }
    else {
        Expr();
        consome(END_EXPRESSION, 0);
    }
}

void Cmd_if() {
    consome(RESERVED_WORD, PR_IF);
    consome(SN, ABRE_PARENTESES);
    
    TIPO tipo_condicao = Expr();
    
    if (tipo_condicao != BOOL_) {
        if (tipo_condicao != INT_) { 
            erro_semantico("A expressao na condicional de um 'if' deve ser do tipo booleano.", contLinha);
        }
    }
    
    consome(SN, FECHA_PARENTESES);
    Cmd();
    if (t.cat == RESERVED_WORD && t.codigo == PR_ELSE) {
        consome(RESERVED_WORD, PR_ELSE);
        Cmd();
    }
}

void Cmd_while() {
    consome(RESERVED_WORD, PR_WHILE);
    consome(SN, ABRE_PARENTESES);
    
    TIPO tipo_condicao = Expr();
    
    if (tipo_condicao != BOOL_ && tipo_condicao != INT_) {
        erro_semantico("A expressao na condicional de um 'while' deve ser do tipo booleano ou inteiro.", contLinha);
    }

    consome(SN, FECHA_PARENTESES);
    Cmd();
}

void Cmd_for() {
    consome(RESERVED_WORD, PR_FOR);
    consome(SN, ABRE_PARENTESES);
    
    if (t.cat != END_EXPRESSION) {
        Expr();
    }
    consome(END_EXPRESSION, 0);
    

    if (t.cat != END_EXPRESSION) {
        TIPO tipo_condicao = Expr();
        
        if (tipo_condicao != BOOL_ && tipo_condicao != INT_) {
            erro_semantico("A expressao na condicional de um 'for' deve ser do tipo booleano ou inteiro.", contLinha);
        }
    }
    consome(END_EXPRESSION, 0);

    if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
        Expr();
    }
    consome(SN, FECHA_PARENTESES);
    
    Cmd(); 
}

void Cmd_return() {
    consome(RESERVED_WORD, PR_RETURN);
    if (t.cat != END_EXPRESSION) Expr();
    consome(END_EXPRESSION, 0);
}

void Cmd_break() {
    consome(RESERVED_WORD, PR_BREAK);
    consome(END_EXPRESSION, 0);
}

void Cmd_continue() {
    consome(RESERVED_WORD, PR_CONTINUE);
    consome(END_EXPRESSION, 0);
}

void Cmd_bloco() {
    consome(SN, ABRE_CHAVE);

    while (!(t.cat == SN && t.codigo == FECHA_CHAVE) && t.cat != END_FILE) {
        Cmd();
    }
    consome(SN, FECHA_CHAVE);
}

// --- Funções para Análise de Expressões ---
// Seguem a estrutura de precedência de operadores.

void Expr() {
    Expr_atrib();
}

void Expr_atrib() {
    Expr_ou();
    if (t.cat == SN && t.codigo == OP_ATRIBUICAO) {
        consome(SN, OP_ATRIBUICAO);
        Expr_atrib();
    }
}

void Expr_ou() {
    Expr_e();
    while (t.cat == SN && t.codigo == OP_OR) {
        consome(SN, OP_OR);
        Expr_e();
    }
}

void Expr_e() {
    Expr_relacional();
    while (t.cat == SN && t.codigo == OP_AND) {
        consome(SN, OP_AND);
        Expr_relacional();
    }
}

TIPO Expr_relacional() {
    TIPO tipo_esq = Expr_aditiva();
     if (t.cat == SN && (t.codigo == OP_IGUAL || t.codigo == OP_DIFERENTE || /* ... outros ... */)) {
        consome(SN, t.codigo);
        TIPO tipo_dir = Expr_aditiva();
        
        if (!((tipo_esq == INT_ || tipo_esq == CHAR_ || tipo_esq == REAL_) && tipo_esq == tipo_dir)) {
             erro_semantico("Tipos incompativeis para operacao relacional.", contLinha);
        }
        
        return BOOL_; 
    }
    return tipo_esq;
}

TIPO Expr_aditiva() {
    TIPO tipo_esq = Expr_multiplicativa(); // Pega o tipo do primeiro operando
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        consome(SN, t.codigo);
        TIPO tipo_dir = Expr_multiplicativa(); // Pega o tipo do segundo operando

        // --- CHECAGEM DE TIPO SEMÂNTICA ---

        if ((tipo_esq == INT_ || tipo_esq == CHAR_) && (tipo_dir == INT_ || tipo_dir == CHAR_)) {
            tipo_esq = INT_; 
        }
        else if (tipo_esq == REAL_ && tipo_dir == REAL_) {
            tipo_esq = REAL_
        }
        else {
            erro_semantico("Tipos incompativeis para operacao de adicao/subtracao.", contLinha);
            tipo_esq = NA_TIPO; // Retorna um tipo de erro
        }
    }
    return tipo_esq;
}

void Expr_multiplicativa() {
    Fator();
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        consome(SN, t.codigo);
        Fator();
    }
}

/**
 * @brief Analisa um fator e RETORNA O SEU TIPO.
 */
TIPO Fator() {
    TIPO tipo_retorno = NA_TIPO; // Inicia com um tipo de erro padrão.

    // Trata operadores unários como '!'
    if (t.cat == SN && t.codigo == OP_NOT) {
        consome(SN, t.codigo);
        TIPO tipo_fator = Fator();
        [cite_start]// Regra Semântica: O operando de '!' deve ser booleano (ou compatível com int) [cite: 143]
        if (tipo_fator != BOOL_ && tipo_fator != INT_) {
            erro_semantico("O operador '!' exige um operando do tipo booleano ou inteiro.", contLinha);
        }
        return BOOL_; // O resultado de '!' [cite_start]é sempre booleano [cite: 137]
    }
    // Opcional: Tratar '+' e '-' unários aqui se a gramática os colocasse em 'fator'

    // Trata um ID (variável, constante, chamada de função, acesso a array)
    else if (t.cat == ID) {
        // --- VERIFICAÇÃO DE USO DE IDENTIFICADOR ---
        SIMBOLO* s = busca_simbolo(&ts, t.lexema);
        if (s == NULL) {
            char msg[200];
            sprintf(msg, "Identificador '%s' nao foi declarado.", t.lexema);
            erro_semantico(msg, contLinha);
            tipo_retorno = NA_TIPO; // Símbolo não existe, tipo desconhecido
        } else {
            tipo_retorno = s->tipo; [cite_start]// Tipo base é o do símbolo na tabela [cite: 131]
        }
        
        char id_usado[100];
        strcpy(id_usado, t.lexema); 
        
        consome(ID, 0);

        // Verifica se é um acesso a array
        if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
            [cite_start]// Regra Semântica: O tipo do índice deve ser compatível com int [cite: 140]
            consome(SN, ABRE_COLCHETE);
            TIPO tipo_indice = Expr();
            if (tipo_indice != INT_ && tipo_indice != CHAR_) {
                erro_semantico("O indice de um array deve ser um inteiro.", contLinha);
            }
            consome(SN, FECHA_COLCHETE);
            [cite_start]// O tipo da expressão id[expr] é o tipo base do array [cite: 132]
            // tipo_retorno já foi definido para o tipo do símbolo, o que está correto.
        } 
        // Verifica se é uma chamada de função
        else if (t.cat == SN && t.codigo == ABRE_PARENTESES) { 
            if (s != NULL && s->categoria != CAT_FUNC && s->categoria != CAT_PROT) {
                char msg[200];
                sprintf(msg, "'%s' nao e uma funcao e nao pode ser chamada.", id_usado);
                erro_semantico(msg, contLinha);
            }
            [cite_start]// O tipo da expressão id(...) é o tipo de retorno da função [cite: 133]
            // tipo_retorno já foi definido para o tipo do símbolo, que é o tipo de retorno.

            consome(SN, ABRE_PARENTESES);
            if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
                // A checagem de número e tipo de parâmetros seria feita aqui
                Expr();
                while (t.cat == SN && t.codigo == VIRGULA) {
                    consome(SN, VIRGULA);
                    Expr();
                }
            }
            consome(SN, FECHA_PARENTESES);
        }
    } 
    // Trata constantes
    else if (t.cat == CT_INT) {
        consome(t.cat, 0);
        tipo_retorno = INT_; [cite_start]// O tipo de uma constante inteira é int [cite: 130]
    } else if (t.cat == CT_REAL) {
        consome(t.cat, 0);
        tipo_retorno = REAL_;
    } else if (t.cat == CT_CHAR) {
        consome(t.cat, 0);
        tipo_retorno = CHAR_;
    }
    // Trata uma expressão entre parênteses
    else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        consome(SN, ABRE_PARENTESES);
        tipo_retorno = Expr(); // O tipo do fator é o tipo da expressão interna
        consome(SN, FECHA_PARENTESES);
    } 
    // Se não for nada do esperado, é um erro
    else {
        error("Expressao mal formada no fator.");
    }
    
    return tipo_retorno;
}