// Analisador Sintático Expandido para linguagem Cshort
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "anaSint.h"
#include "../lex/anaLex.h"

bool modoPanico = false; // Variável para controlar a recuperação de erro
bool houveErroSintatico = false;

// --- Protótipos ---
void Prog();
void Decl();
void Decl_var();
int  Tipo();
void Tipos_param();
void Cmd();
void Cmd_if();
void Cmd_while();
void Cmd_for();
void Cmd_return();
void Cmd_bloco();
void Cmd_break();
void Cmd_continue();
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

char TABS[200] = "";
FILE *fd;
TOKEN t;

void error(char msg[]) {
    printf("[ERRO | Linha %d]: %s\n", contLinha, msg);
    modoPanico = true; 
    houveErroSintatico = true;
}

void aumenta_ident() {
    strcat(TABS, "  ");
}

void diminui_ident() {
    int len = strlen(TABS);
    if (len >= 2) TABS[len - 2] = '\0';
}

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

// Função para sincronizar o parser após um erro
void sincroniza() {
    // Descarta tokens até encontrar um ponto seguro para recomeçar
    while (t.cat != END_FILE) {
        // Pontos de sincronização: ; } ou início de um novo comando
        if (t.cat == END_EXPRESSION) { // Ponto e vírgula
            t = AnaLex(fd);
            break;
        }
        if (t.cat == SN && t.codigo == FECHA_CHAVE) { // Fim de bloco
            break; 
        }
        if (t.cat == RESERVED_WORD) {
            switch(t.codigo) {
                case PR_IF:
                case PR_WHILE:
                case PR_FOR:
                case PR_RETURN:
                case PR_INT:
                case PR_FLOAT:
                case PR_CHAR:
                case PR_STRING:
                    goto end_loop; // Sai dos dois laços
            }
        }
        t = AnaLex(fd);
    }
end_loop:
    modoPanico = false; // Desativa o modo pânico após sincronizar
}

void consome(int categoria, int codigo) {
    if (modoPanico) return; 

    if (t.cat == categoria && (codigo == 0 || t.codigo == codigo)) {
        print_folha(t);
        t = AnaLex(fd);
    } else {
        char msg[200];
        sprintf(msg, "Esperado (cat=%d, cod=%d), encontrado token '%s' (cat=%d, cod=%d)", 
                categoria, codigo, t.lexema, t.cat, t.codigo);
        error(msg);
    }
}

int Tipo() {
    if (t.cat == RESERVED_WORD) {
        switch (t.codigo) {
            case PR_INT:
            case PR_FLOAT:
            case PR_CHAR:
            case PR_STRING:
                return 1;
        }
    }
    return 0;
}

void Prog() {
    printf("<Prog>\n"); aumenta_ident();
    t = AnaLex(fd);
    while (t.cat != END_FILE) {
        if (Tipo()) {
            Decl_ou_Func();
        } else {
            error("Esperado tipo no inicio de declaracao ou funcao");

            t = AnaLex(fd);
        }
    }
    diminui_ident(); printf("</Prog>\n");
}

void Decl_ou_Func() {
    printf("%s<Decl_ou_Func>\n", TABS); aumenta_ident();
    print_folha(t); consome(t.cat, t.codigo); // tipo
    print_folha(t); consome(ID, 0); // identificador
    if (t.cat == SN && t.codigo == ABRE_PARENTESES)
        Func_body();
    else
        Decl_var_body();
    diminui_ident(); printf("%s</Decl_ou_Func>\n", TABS);
}

void Func_body() {
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Tipos_param();
    print_folha(t); consome(SN, FECHA_PARENTESES);
    Cmd_bloco();
}

void Decl_var_body() {
    if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
        print_folha(t); consome(SN, ABRE_COLCHETE);
        print_folha(t); consome(CT_INT, 0);
        print_folha(t); consome(SN, FECHA_COLCHETE);
    }
    while (t.cat == SN && t.codigo == VIRGULA) {
        print_folha(t); consome(SN, VIRGULA);
        print_folha(t); consome(ID, 0);
    }
    print_folha(t); consome(END_EXPRESSION, 0);
}

void Decl() {
    if (Tipo()) {
        print_folha(t); consome(t.cat, t.codigo);
        print_folha(t); consome(ID, 0);
        while (t.cat == SN && t.codigo == VIRGULA) {
            print_folha(t); consome(SN, VIRGULA);
            print_folha(t); consome(ID, 0);
        }
        print_folha(t); consome(END_EXPRESSION, 0);
    } else error("Esperado tipo na declaracao");
}

void Tipos_param() {
    if (t.cat == RESERVED_WORD) {
        while (true) {
            if (!Tipo()) break;
            print_folha(t); consome(t.cat, t.codigo);
            print_folha(t); consome(ID, 0);
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                print_folha(t); consome(SN, ABRE_COLCHETE);
                print_folha(t); consome(SN, FECHA_COLCHETE);
            }
            if (t.cat != SN || t.codigo != VIRGULA) break;
            print_folha(t); consome(SN, VIRGULA);
        }
    }
}

void Cmd() {
    if (t.cat == RESERVED_WORD && t.codigo == PR_IF) Cmd_if();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_WHILE) Cmd_while();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_FOR) Cmd_for();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) Cmd_return();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_BREAK) Cmd_break();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_CONTINUE) Cmd_continue();
    else if (t.cat == SN && t.codigo == ABRE_CHAVE) Cmd_bloco();
    else {
        Expr();
        print_folha(t); consome(END_EXPRESSION, 0);
    }
}

void Cmd_if() {
    print_folha(t); consome(RESERVED_WORD, PR_IF);
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Expr();
    print_folha(t); consome(SN, FECHA_PARENTESES);
    Cmd();
    if (t.cat == RESERVED_WORD && t.codigo == PR_ELSE) {
        print_folha(t); consome(RESERVED_WORD, PR_ELSE);
        Cmd();
    }
}

void Cmd_while() {
    print_folha(t); consome(RESERVED_WORD, PR_WHILE);
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Expr();
    print_folha(t); consome(SN, FECHA_PARENTESES);
    Cmd();
}

void Cmd_for() {
    print_folha(t); consome(RESERVED_WORD, PR_FOR);
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Expr(); print_folha(t); consome(END_EXPRESSION, 0);
    Expr(); print_folha(t); consome(END_EXPRESSION, 0);
    Expr(); print_folha(t); consome(SN, FECHA_PARENTESES);
    Cmd();
}

void Cmd_return() {
    print_folha(t); consome(RESERVED_WORD, PR_RETURN);
    if (t.cat != END_EXPRESSION) Expr();
    print_folha(t); consome(END_EXPRESSION, 0);
}

void Cmd_break() {
    print_folha(t); consome(RESERVED_WORD, PR_BREAK);
    print_folha(t); consome(END_EXPRESSION, 0);
}

void Cmd_continue() {
    print_folha(t); consome(RESERVED_WORD, PR_CONTINUE);
    print_folha(t); consome(END_EXPRESSION, 0);
}

void Cmd_bloco() {
    print_folha(t); consome(SN, ABRE_CHAVE);
    while (!(t.cat == SN && t.codigo == FECHA_CHAVE) && t.cat != END_FILE) {
        if (modoPanico) {
            sincroniza();
            // Após sincronizar, podemos estar no '}' final, então verificamos de novo
            if (t.cat == SN && t.codigo == FECHA_CHAVE) break;
        }
        Cmd();
    }
    print_folha(t); consome(SN, FECHA_CHAVE);
}

void Expr() {
    Expr_atrib();
}

void Expr_atrib() {
    Expr_ou();
    if (t.cat == SN && t.codigo == OP_ATRIBUICAO) {
        print_folha(t); consome(SN, OP_ATRIBUICAO);
        Expr_atrib();
    }
}

void Expr_ou() {
    Expr_e();
    while (t.cat == SN && t.codigo == OP_OR) {
        print_folha(t); consome(SN, OP_OR);
        Expr_e();
    }
}

void Expr_e() {
    Expr_relacional();
    while (t.cat == SN && t.codigo == OP_AND) {
        print_folha(t); consome(SN, OP_AND);
        Expr_relacional();
    }
}

void Expr_relacional() {
    Expr_aditiva();
    while (t.cat == SN && (t.codigo == OP_IGUAL || t.codigo == OP_DIFERENTE || t.codigo == OP_MAIOR || t.codigo == OP_MAIOR_IGUAL || t.codigo == OP_MENOR || t.codigo == OP_MENOR_IGUAL)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_aditiva();
    }
}

void Expr_aditiva() {
    Expr_multiplicativa();
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_multiplicativa();
    }
}

void Expr_multiplicativa() {
    Fator();
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        print_folha(t); consome(SN, t.codigo);
        Fator();
    }
}

void Fator() {
    if (t.cat == SN && (t.codigo == OP_NOT || t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        print_folha(t); consome(SN, t.codigo);
        Fator();
    } else if (t.cat == ID) {
        print_folha(t); consome(ID, 0);
        if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
            print_folha(t); consome(SN, ABRE_COLCHETE);
            Expr();
            print_folha(t); consome(SN, FECHA_COLCHETE);
        } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
            print_folha(t); consome(SN, ABRE_PARENTESES);
            if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
                Expr();
                while (t.cat == SN && t.codigo == VIRGULA) {
                    print_folha(t); consome(SN, VIRGULA);
                    Expr();
                }
            }
            print_folha(t); consome(SN, FECHA_PARENTESES);
        }
    } else if (t.cat == CT_INT || t.cat == CT_REAL || t.cat == CT_CHAR || t.cat == CT_STRING) {
        print_folha(t); consome(t.cat, 0);
    } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
    } else {
        error("Expressão mal formada no fator.");
    }
}
