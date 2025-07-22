/**
 * @file anaSint.c
 * @brief Implementação do Analisador Sintático e Semântico.
 *
 * @purpose
 * Este arquivo contém a lógica completa do analisador de descida recursiva.
 * Ele implementa as regras da gramática da linguagem Cshort e integra as
 * ações semânticas, como o gerenciamento de escopo e a checagem de tipos,
 * através da interação com o módulo da Tabela de Símbolos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "anaSint.h"

// --- Definição das Variáveis Globais ---
FILE *fd;
TOKEN t, tLookahead;
TabelaSimbolos ts;
bool houveErroSintatico = false;
char TABS[200] = "";

SIMBOLO* funcao_atual = NULL;

// --- Implementação das Funções Auxiliares ---

/**
 * @brief Reporta um erro semântico.
 * @param msg Mensagem de erro.
 * @param linha Número da linha onde o erro ocorreu.
 */
void erro_semantico(char* msg, int linha) {
    printf("[ERRO SEMANTICO | Linha %d]: %s\n", linha, msg);
    houveErroSintatico = true;
    exit(1); // Para no primeiro erro
}

/**
 * @brief Reporta um erro sintático.
 * @param msg Mensagem de erro a ser exibida.
 */
void error(char* msg) {
    printf("[ERRO SINTATICO | Linha %d]: %s\n", contLinha, msg);
    houveErroSintatico = true;
    exit(1); // Para no primeiro erro
}

void aumenta_ident() { strcat(TABS, "  "); }
void diminui_ident() { int len = strlen(TABS); if (len >= 2) TABS[len - 2] = '\0'; }

void nextToken() {
    t = tLookahead;
    if (t.cat != END_FILE) {
        tLookahead = AnaLex(fd);
    }
}

void consome(int categoria, int codigo) {
    if ((int)t.cat == categoria && (codigo == 0 || t.codigo == codigo)) {
        nextToken();
    } else {
        char msg[200];
        sprintf(msg, "Token inesperado. Esperado (cat=%d, cod=%d), encontrado '%s'.", categoria, codigo, t.lexema);
        error(msg);
    }
}

// --- Implementação das Funções do Parser ---

int Tipo() {
    if (t.cat == RESERVED_WORD) {
        switch (t.codigo) {
            case PR_INT: case PR_FLOAT: case PR_CHAR: case PR_VOID: case PR_STRING: case PR_BOOL:
                return 1;
        }
    }
    return 0;
}

// Converte um tipo do léxico (PR_INT) para um tipo semântico (INT_)
TIPO pr_to_tipo(int pr_code) {
    switch (pr_code) {
        case PR_INT: return INT_;
        case PR_FLOAT: return REAL_;
        case PR_CHAR: return CHAR_;
        case PR_BOOL: return BOOL_;
        case PR_VOID: return VOID_;
        default: return NA_TIPO;
    }
}

void Prog(char* nomeArquivo) {
    fd = fopen(nomeArquivo, "r");
    if (!fd) {
        printf("Nao consegui abrir o arquivo '%s'.\n", nomeArquivo);
        exit(1);
    }

    ts = inicializa_tabela(); 
    tLookahead = AnaLex(fd);
    nextToken();

    while (t.cat != END_FILE) {
        SIMBOLO* func_simbolo = NULL;
        DECL_SINALIZADOR flag = Decl(&func_simbolo);
        if (flag == DECL_FUNC) { 
            corpo_func(func_simbolo); 
        }
    }
    imprime_tabela(ts);
    salvar_codigo_em_arquivo("out/codigo_gerado.maq"); 
    fclose(fd);
}

/**
 * @brief Função auxiliar para emitir uma instrução para a máquina de pilha.
 * @param op A operação (LOAD, ADD)
 * @param arg O argumento da operação 
 */

void gera_codigo(char* op, char* arg) {
    if (arg != NULL) {
        printf("%s %s\n", op, arg);
    } else {
        printf("%s\n", op);
    }
}


void corpo_func(SIMBOLO* func_simbolo) {
    funcao_atual = func_simbolo;
    abre_escopo(&ts);
    Cmd_bloco();
    fecha_escopo(&ts); 

    funcao_atual = NULL;
}

DECL_SINALIZADOR Decl(SIMBOLO** func_encontrada) {
    DECL_SINALIZADOR declFlag = NO_DECL;
    if (!Tipo()) {
        error("Esperado um tipo no inicio da declaracao.");
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
        s.tipo = pr_to_tipo(tipo_declarado);
        s.categoria = CAT_FUNC;
        s.info.func.num_parametros = 0;
        if (!insere_simbolo(&ts, s)) {
            char msg[200];
            sprintf(msg, "Redeclaracao da funcao '%s'.", s.id);
            erro_semantico(msg, contLinha);
        }

        *func_encontrada = busca_simbolo(&ts, nome_id);

        nextToken();
        nextToken();
        abre_escopo(&ts);
        Tipos_param(nome_id); 
        consome(SN, FECHA_PARENTESES);
        
        if (t.cat == SN && t.codigo == ABRE_CHAVE) {
            declFlag = DECL_FUNC; 
        } else {
            declFlag = DECL_PROT; 
            SIMBOLO* func_s = busca_simbolo(&ts, nome_id);
            if(func_s) func_s->categoria = CAT_PROT;
            fecha_escopo(&ts); 
            consome(END_EXPRESSION, 0);
        }
    } else {
        declFlag = DECL_VAR;
        if (tipo_declarado == PR_VOID) {
            erro_semantico("Variaveis nao podem ser do tipo 'void'.", contLinha);
        }
        while(true) {
            if (t.cat != ID) { error("Identificador de variavel esperado."); break; }
            
            SIMBOLO s;
            strcpy(s.id, t.lexema);
            s.tipo = pr_to_tipo(tipo_declarado);
            s.categoria = CAT_VAR;
            if (!insere_simbolo(&ts, s)) {
                char msg[200];
                sprintf(msg, "Redeclaracao da variavel '%s'.", s.id);
                erro_semantico(msg, contLinha);
            }
            
            nextToken();
            
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) { nextToken(); consome(CT_INT, 0); consome(SN, FECHA_COLCHETE); }
            if (t.cat == SN && t.codigo == VIRGULA) { nextToken(); } else { break; }
        }
        consome(END_EXPRESSION, 0);
    }
    return declFlag;
}

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
            int tipo_param_lexico = t.codigo;
            consome(t.cat, t.codigo);
            
            SIMBOLO p;
            p.tipo = pr_to_tipo(tipo_param_lexico);
            p.categoria = CAT_PARAM;
            set_tipo_param(&ts, nome_funcao, tipo_param_lexico);

            if (t.cat == ID) {
                strcpy(p.id, t.lexema);
                if(!insere_simbolo(&ts, p)) {
                    char msg[200];
                    sprintf(msg, "Redeclaracao do parametro '%s'.", p.id);
                    erro_semantico(msg, contLinha);
                }
                incrementa_num_param(&ts, nome_funcao);
                consome(ID, 0);
            } else {
                strcpy(p.id, ""); 
                incrementa_num_param(&ts, nome_funcao);
            }
            
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) { consome(SN, ABRE_COLCHETE); consome(SN, FECHA_COLCHETE); }
            if (t.cat != SN || t.codigo != VIRGULA) break;
            consome(SN, VIRGULA);
        }
    }
}

void Cmd() {
    if (Tipo()) { 
        SIMBOLO* simbolo_local = NULL;
        Decl(&simbolo_local); 
        
    }else if (t.cat == RESERVED_WORD && t.codigo == PR_IF) Cmd_if();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_WHILE) Cmd_while();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_FOR) Cmd_for();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) Cmd_return();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_BREAK) Cmd_break();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_CONTINUE) Cmd_continue();
    else if (t.cat == SN && t.codigo == ABRE_CHAVE) { abre_escopo(&ts); Cmd_bloco(); fecha_escopo(&ts); }
    else if (t.cat == ID && tLookahead.cat == SN && tLookahead.codigo == OP_ATRIBUICAO) {Cmd_atrib();}
    else { Expr(); consome(END_EXPRESSION, 0); }
}

void Cmd_bloco() {
    consome(SN, ABRE_CHAVE);
    while (t.cat != SN || t.codigo != FECHA_CHAVE) {
        Cmd();
    }
    consome(SN, FECHA_CHAVE);
}

void Cmd_if() {
    consome(RESERVED_WORD, PR_IF);
    consome(SN, ABRE_PARENTESES);

    // Atividade 3: DDS que verifica o tipo da expressão
    TIPO tipo_condicao = Expr(); // A chamada a Expr() também gera o código para a condição
    if (tipo_condicao != BOOL_ && tipo_condicao != INT_) {
        erro_semantico("A expressao na condicional de um 'if' deve ser do tipo booleano ou inteiro.", contLinha);
    }

    consome(SN, FECHA_PARENTESES);

    // Atividade 1: Esquema de tradução para if/if-else.
    int rotulo_else = novo_rotulo();
    int rotulo_fim;
    char instrucao[TAM_LINHA];

    snprintf(instrucao, sizeof(instrucao), "JUMP_FALSE L%d", rotulo_else);
    gera(instrucao);

    Cmd();

    if (t.cat == RESERVED_WORD && t.codigo == PR_ELSE) {
        consome(RESERVED_WORD, PR_ELSE);
        rotulo_fim = novo_rotulo();
        snprintf(instrucao, sizeof(instrucao), "JUMP L%d", rotulo_fim);
        gera(instrucao);
        
        gera_rotulo(rotulo_else);
        Cmd();
        gera_rotulo(rotulo_fim);
    } else {
        gera_rotulo(rotulo_else);
    }
}


/**
 * @brief Analisa 'while', verifica o tipo da condição e gera código de laço.
 */
void Cmd_while() {
    consome(RESERVED_WORD, PR_WHILE);
    
    // Atividade 1: Prepara os rótulos do laço.
    int rotulo_inicio = novo_rotulo();
    int rotulo_fim = novo_rotulo();
    char instrucao[TAM_LINHA];

    gera_rotulo(rotulo_inicio);
    
    consome(SN, ABRE_PARENTESES);
    
    // Atividade 3: Verifica o tipo da condição.
    TIPO tipo_condicao = Expr();
    if (tipo_condicao != BOOL_ && tipo_condicao != INT_) {
        erro_semantico("A expressao na condicional de um 'while' deve ser booleana ou inteira.", contLinha);
    }
    
    consome(SN, FECHA_PARENTESES);

    // Atividade 1: Se a condição for falsa, sai do laço.
    snprintf(instrucao, sizeof(instrucao), "JUMP_FALSE L%d", rotulo_fim);
    gera(instrucao);

    Cmd();

    // Atividade 1: Volta para o início para reavaliar a condição.
    snprintf(instrucao, sizeof(instrucao), "JUMP L%d", rotulo_inicio);
    gera(instrucao);

    gera_rotulo(rotulo_fim);
}


void Cmd_for() {
    consome(RESERVED_WORD, PR_FOR); consome(SN, ABRE_PARENTESES);
    if(t.cat != END_EXPRESSION) Expr(); 
    consome(END_EXPRESSION, 0);
    if (t.cat != END_EXPRESSION) {
        TIPO tipo_cond = Expr();
        if (tipo_cond != BOOL_ && tipo_cond != INT_) {
            erro_semantico("A expressao na condicional de um 'for' deve ser do tipo booleano ou inteiro.", contLinha);
        }
    }
    consome(END_EXPRESSION, 0);
    if(t.cat != SN || t.codigo != FECHA_PARENTESES) Expr();
    consome(SN, FECHA_PARENTESES); Cmd();
}

void Cmd_return() {
    consome(RESERVED_WORD, PR_RETURN);
    
    TIPO tipo_retornado = VOID_; 

    if (t.cat != END_EXPRESSION) {
        tipo_retornado = Expr(); 
    }

    // Verificação de Tipos do Retorno
    if (funcao_atual != NULL) {
        if (funcao_atual->tipo == VOID_ && tipo_retornado != VOID_) {
            erro_semantico("Funcao do tipo 'void' nao pode retornar um valor.", contLinha);
        }
        else if (funcao_atual->tipo != VOID_ && tipo_retornado == VOID_) {
            erro_semantico("Funcao nao-void deve retornar um valor.", contLinha);
        }
        else if (funcao_atual->tipo != tipo_retornado) {
            if (!((funcao_atual->tipo == INT_ && tipo_retornado == CHAR_) || (funcao_atual->tipo == CHAR_ && tipo_retornado == INT_))) {
                erro_semantico("O tipo do valor de retorno e incompativel com o tipo declarado da funcao.", contLinha);
            }
        }
    }
    
    consome(END_EXPRESSION, 0);
}
void Cmd_break() { consome(RESERVED_WORD, PR_BREAK); consome(END_EXPRESSION, 0); }
void Cmd_continue() { consome(RESERVED_WORD, PR_CONTINUE); consome(END_EXPRESSION, 0); }

/**
 * @brief Analisa um comando de atribuição, realiza a checagem de tipos
 * e gera o código de máquina de pilha.
 */

void Cmd_atrib() {
    SIMBOLO* s = busca_simbolo(&ts, t.lexema);
    if (s == NULL) {
        char msg[200];
        sprintf(msg, "Variavel '%s' nao foi declarada.", t.lexema);
        erro_semantico(msg, contLinha);
    }
    
    // Guarda o tipo e o nome da variável
    TIPO tipo_esq = s->tipo;
    char nome_var[100];
    strcpy(nome_var, t.lexema);
    
    consome(ID, 0); // Consome o ID da variável
    consome(SN, OP_ATRIBUICAO); // Consome o '='
    
    TIPO tipo_dir = Expr();
    
    // Verifica a compatibilidade de tipos para a atribuição
    if (tipo_esq != tipo_dir && !(tipo_esq == INT_ && tipo_dir == CHAR_) && !(tipo_esq == CHAR_ && tipo_dir == INT_)) {
        erro_semantico("Tipos incompativeis para atribuicao.", contLinha);
    }

    // Gera a instrução para armazenar o resultado
    gera_codigo("STORE", nome_var);

    consome(END_EXPRESSION, 0);
}
TIPO Expr() { return Expr_ou(); }

TIPO Expr_atrib() {
    TIPO tipo_esq = Expr_ou();
    if (t.cat == SN && t.codigo == OP_ATRIBUICAO) {
        consome(SN, OP_ATRIBUICAO);
        TIPO tipo_dir = Expr_atrib(); 
        if (tipo_esq != tipo_dir && !(tipo_esq == INT_ && tipo_dir == CHAR_) && !(tipo_esq == CHAR_ && tipo_dir == INT_)) { 
            erro_semantico("Tipos incompativeis para atribuicao.", contLinha);
        }
        return tipo_esq;
    }
    return tipo_esq;
}

TIPO Expr_ou() {
    TIPO tipo_esq = Expr_e();
    while (t.cat == SN && t.codigo == OP_OR) {
        consome(SN, OP_OR);
        TIPO tipo_dir = Expr_e(); 
        if ((tipo_esq != BOOL_ && tipo_esq != INT_) || (tipo_dir != BOOL_ && tipo_dir != INT_)) { 
            erro_semantico("Operador '||' exige operandos do tipo booleano ou inteiro.", contLinha);
        }
        tipo_esq = BOOL_; 
    }
    return tipo_esq;
}

TIPO Expr_e() {
    TIPO tipo_esq = Expr_relacional();
    while (t.cat == SN && t.codigo == OP_AND) {
        consome(SN, OP_AND);
        TIPO tipo_dir = Expr_relacional(); 
        if ((tipo_esq != BOOL_ && tipo_esq != INT_) || (tipo_dir != BOOL_ && tipo_dir != INT_)) {
            erro_semantico("Operador '&&' exige operandos do tipo booleano ou inteiro.", contLinha);
        }
        tipo_esq = BOOL_;
    }
    return tipo_esq;
}

TIPO Expr_relacional() {
    TIPO tipo_esq = Expr_aditiva();

    if (t.cat == SN && (t.codigo >= OP_MAIOR && t.codigo <= OP_DIFERENTE)) {
        int op = t.codigo;
        consome(SN, t.codigo);
        TIPO tipo_dir = Expr_aditiva();

        if (tipo_esq != tipo_dir) { 
            erro_semantico("Tipos incompativeis para operacao relacional.", contLinha); 
        }

        switch (op) {
            case OP_IGUAL:       gera("EQ"); break;
            case OP_DIFERENTE:   gera("NE"); break;
            case OP_MAIOR:       gera("GT"); break;
            case OP_MAIOR_IGUAL: gera("GTE"); break;
            case OP_MENOR:       gera("LT"); break;
            case OP_MENOR_IGUAL: gera("LTE"); break;
        }
        
        return BOOL_;
    }
    
    return tipo_esq;
}

/**
 * @brief Analisa expressões aritméticas e relacionais, checando tipos e gerando código.
 */

TIPO Expr_aditiva() {
    TIPO tipo_esq = Expr_multiplicativa();
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        int op = t.codigo;
        consome(SN, t.codigo);
        TIPO tipo_dir = Expr_multiplicativa();
        
        // Atividade 3: Checagem de compatibilidade de tipos.
        if ((tipo_esq == REAL_ && tipo_dir != REAL_) || (tipo_esq != REAL_ && tipo_dir == REAL_)) {
            erro_semantico("Tipos incompativeis.", contLinha);
        }
        
        // Atividade 2: Gera a instrução aritmética correspondente.
        if (op == OP_SOMA) gera("ADD"); else gera("SUB");
        
        if (tipo_esq == REAL_ || tipo_dir == REAL_) tipo_esq = REAL_; 
        else tipo_esq = INT_; 
    }
    return tipo_esq;
}

TIPO Expr_multiplicativa() {
    TIPO tipo_esq = Fator();
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        int op = t.codigo;
        consome(SN, t.codigo);
        TIPO tipo_dir = Fator();
        if ((tipo_esq == REAL_ && tipo_dir != REAL_) || (tipo_esq != REAL_ && tipo_dir == REAL_)) { 
            erro_semantico("Tipos incompativeis para operacao (float com int/char).", contLinha);
            return NA_TIPO; 
        }
        if (op == OP_MULTIPLICACAO) gera_codigo("MUL", NULL); else gera_codigo("DIV", NULL);
        if (tipo_esq == REAL_ || tipo_dir == REAL_) tipo_esq = REAL_;
        else tipo_esq = INT_;
    }
    return tipo_esq;
}

/**
 * @brief Analisa um fator, o elemento base de uma expressão.
 *
 * @return TIPO O tipo semântico do fator analisado.
 */

TIPO Fator() {

    TIPO tipo_retorno = NA_TIPO;
    char instrucao[100]; 

    if (t.cat == ID) {
        SIMBOLO* s = busca_simbolo(&ts, t.lexema);
        
        if (s == NULL) {
            char msg[200];
            sprintf(msg, "Identificador '%s' nao foi declarado.", t.lexema);
            erro_semantico(msg, contLinha);
            consome(ID, 0); // Consome o token para evitar laços infinitos
            return NA_TIPO;
        }
        
        tipo_retorno = s->tipo;

        snprintf(instrucao, sizeof(instrucao), "LOAD %s", t.lexema);
        gera(instrucao);

        char id_usado[100];
        strcpy(id_usado, t.lexema);
        consome(ID, 0);

        // Se o ID é seguido por (
        if (t.cat == SN && t.codigo == ABRE_PARENTESES) { 
            // Verifica se o símbolo é uma função
            if (s->categoria != CAT_FUNC && s->categoria != CAT_PROT) {
                char msg[200];
                sprintf(msg, "'%s' nao e uma funcao e nao pode ser chamada.", id_usado);
                erro_semantico(msg, contLinha);
            }
            consome(SN, ABRE_PARENTESES);

            int arg_count = 0;
            TIPO arg_tipos[10]; 

            if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
                // Analisa o primeiro argumento
                arg_tipos[arg_count++] = Expr();

                // Analisa os argumentos seguintes
                while (t.cat == SN && t.codigo == VIRGULA) {
                    consome(SN, VIRGULA);
                    if (arg_count < 10) {
                        arg_tipos[arg_count++] = Expr();
                    } else {
                        erro_semantico("Numero maximo de argumentos (10) excedido.", contLinha);
                        Expr(); // Consome a expressão para evitar erros em cascata
                    }
            }
            consome(SN, FECHA_PARENTESES);
        }
        if (arg_count != s->info.func.num_parametros) {
                char msg[200];
                sprintf(msg, "Numero incorreto de argumentos para a funcao '%s'. Esperado: %d, Fornecido: %d.", 
                        id_usado, s->info.func.num_parametros, arg_count);
                erro_semantico(msg, contLinha);
            } else {
                // Regra 3.2.2.7.2 (Tipos dos argumentos)
                for (int i = 0; i < arg_count; i++) {
                    if (arg_tipos[i] != s->info.func.tipos_parametros[i]) {
                        // Verificando a compatibilidade int/char
                        if (!((s->info.func.tipos_parametros[i] == INT_ && arg_tipos[i] == CHAR_) ||
                              (s->info.func.tipos_parametros[i] == CHAR_ && arg_tipos[i] == INT_))) {
                            char msg[200];
                            sprintf(msg, "Tipo do argumento %d incompativel na chamada da funcao '%s'.", i + 1, id_usado);
                            erro_semantico(msg, contLinha);
                            break; // Para no primeiro erro de tipo de argumento
                        }
                    }
                }
            }
            
            consome(SN, FECHA_PARENTESES);

        }
        return tipo_retorno;


    } else if (t.cat == CT_INT) {
        snprintf(instrucao, sizeof(instrucao), "PUSH %d", t.int_value);
        gera(instrucao);
        consome(t.cat, 0);
        return INT_;

    } else if (t.cat == CT_REAL) { 
        snprintf(instrucao, sizeof(instrucao), "PUSH %f", t.real_value);
        gera(instrucao);
        consome(t.cat, 0);
        return REAL_;

    } else if (t.cat == CT_CHAR) {
        snprintf(instrucao, sizeof(instrucao), "PUSH %d", t.int_value); // Empilha o valor ASCII
        gera(instrucao);
        consome(t.cat, 0);
        return CHAR_;  

    } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        consome(SN, ABRE_PARENTESES);
        tipo_retorno = Expr();
        consome(SN, FECHA_PARENTESES);
        return tipo_retorno;

    } else {
        error("Expressao mal formada no fator.");
    }
    return NA_TIPO; 
}



