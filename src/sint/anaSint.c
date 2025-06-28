#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "anasint.h"
#include "analex.h"

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

void aumenta_ident(){
    if (strlen(TABS) < sizeof(TABS) - 3){
        strcat(TABS, " ");
    }
}

void diminui_ident() {
    if (strlen(TABS) >= 2) {
        TABS[strlen(TABS) - 2] = '\0';
    }
}

void print_folha(TOKEN tk) {
    printf("%s- ", TABS); // Usa um traço para indicar que é uma folha
    switch (tk.cat) {
        case ID: printf("ID: %s\n", tk.lexema); break;
        case SN: printf("SN: %d\n", tk.codigo); break;
        case CT_INT: printf("CT_INT: %d\n", tk.int_value); break;
        case CT_REAL: printf("CT_REAL: %f\n", tk.real_value); break;
        case RESERVED_WORD: printf("PR: %d\n", tk.codigo); break;
        default: printf("TOKEN (cat %d)\n", tk.cat); break;
    }
}

void consome(int categoria_esperada, int codigo_esperado) {
    if (t.cat == categoria_esperada && (codigo_esperado == 0 || t.codigo == codigo_esperado)) {
        // A impressão agora é feita pela função que chama `consome`, para maior controle
        t = AnaLex(fd);
    } else {
        char msg_erro[200];
        sprintf(msg_erro, "Erro sintatico na linha %d: esperado (cat %d, cod %d), mas encontrei (cat %d, cod %d)",
                contLinha, categoria_esperada, codigo_esperado, t.cat, t.codigo);
        error(msg_erro);
    }
}

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

void Prog() {
    printf("%s<Prog>\n", TABS); aumenta_ident();
    t = AnaLex(fd);
    while (t.cat != END_FILE) {
        if (Tipo()) {
            Decl_ou_Func();
        } else {
            error("Erro sintatico: esperado uma declaracao ou uma funcao.");
        }
    }
    diminui_ident(); printf("%s</Prog>\n", TABS);
}

void Decl_ou_Func() {
    printf("%s<Decl_ou_Func>\n", TABS); aumenta_ident();
    print_folha(t); consome(t.cat, t.codigo);
    print_folha(t); consome(ID, 0);

    if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        Func_body();
    } else {
        Decl_var_body();
    }
    diminui_ident(); printf("%s</Decl_ou_Func>\n", TABS);
}

void Func_body() {
    printf("%s<Func_body>\n", TABS); aumenta_ident();
    print_folha(t); consome(SN, ABRE_PARENTESES);
    Tipos_param();
    print_folha(t); consome(SN, FECHA_PARENTESES);
    print_folha(t); consome(SN, ABRE_CHAVE);
    while (Tipo()) {
        Decl();
    }
    while (!(t.cat == SN && t.codigo == FECHA_CHAVE)) {
        Cmd();
    }
    print_folha(t); consome(SN, FECHA_CHAVE);
    diminui_ident(); printf("%s</Func_body>\n", TABS);
}

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
    print_folha(t); consome(SN, END_EXPRESSION);
    diminui_ident(); printf("%s</Decl_var_body>\n", TABS);
}

void Decl() {
    printf("%s<Decl>\n", TABS); aumenta_ident();
    if (Tipo()) {
        Decl_var(); // Decl_var agora consome o tipo
        while (t.cat == SN && t.codigo == VIRGULA) {
            print_folha(t); consome(SN, VIRGULA);
            print_folha(t); consome(ID, 0); // Consome o ID da próxima variável
        }
        print_folha(t); consome(SN, END_EXPRESSION);
    } else {
        error("Declaracao de variavel local esperada.");
    }
    diminui_ident(); printf("%s</Decl>\n", TABS);
}

void Decl_var() {
    printf("%s<Decl_var>\n", TABS); aumenta_ident();
    print_folha(t); consome(t.cat, t.codigo); // Consome o tipo
    print_folha(t); consome(ID, 0); // Consome o ID
    
    if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
        print_folha(t); consome(SN, ABRE_COLCHETE);
        print_folha(t); consome(CT_INT, 0);
        print_folha(t); consome(SN, FECHA_COLCHETE);
    }
    diminui_ident(); printf("%s</Decl_var>\n", TABS);
}

void Tipos_param() {
    printf("%s<Tipos_param>\n", TABS); aumenta_ident();
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

void Cmd() {
    printf("%s<Cmd>\n", TABS); aumenta_ident();
    if (t.cat == RESERVED_WORD && t.codigo == PR_IF) {
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
        print_folha(t); consome(RESERVED_WORD, PR_WHILE);
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
        Cmd();
    } else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) {
        print_folha(t); consome(RESERVED_WORD, PR_RETURN);
        if (!(t.cat == SN && t.codigo == END_EXPRESSION)) {
            Expr();
        }
        print_folha(t); consome(SN, END_EXPRESSION);
    } else if (t.cat == SN && t.codigo == ABRE_CHAVE) {
        print_folha(t); consome(SN, ABRE_CHAVE);
        while (!(t.cat == SN && t.codigo == FECHA_CHAVE)) {
            Cmd();
        }
        print_folha(t); consome(SN, FECHA_CHAVE);
    } else {
        Expr();
        print_folha(t); consome(SN, END_EXPRESSION);
    }
    diminui_ident(); printf("%s</Cmd>\n", TABS);
}

void Expr() {
    printf("%s<Expr>\n", TABS); aumenta_ident();
    Expr_atrib();
    diminui_ident(); printf("%s</Expr>\n", TABS);
}

void Expr_atrib() {
    printf("%s<Expr_atrib>\n", TABS); aumenta_ident();
    Expr_ou();
    if (t.cat == SN && t.codigo == OP_ATRIBUICAO) {
        print_folha(t); consome(SN, OP_ATRIBUICAO);
        Expr_atrib();
    }
    diminui_ident(); printf("%s</Expr_atrib>\n", TABS);
}

void Expr_ou() {
    printf("%s<Expr_ou>\n", TABS); aumenta_ident();
    Expr_e();
    while (t.cat == SN && t.codigo == OP_OR) {
        print_folha(t); consome(SN, OP_OR);
        Expr_e();
    }
    diminui_ident(); printf("%s</Expr_ou>\n", TABS);
}

void Expr_e() {
    printf("%s<Expr_e>\n", TABS); aumenta_ident();
    Expr_relacional();
    while (t.cat == SN && t.codigo == OP_AND) {
        print_folha(t); consome(SN, OP_AND);
        Expr_relacional();
    }
    diminui_ident(); printf("%s</Expr_e>\n", TABS);
}

void Expr_relacional() {
    printf("%s<Expr_relacional>\n", TABS); aumenta_ident();
    Expr_aditiva();
    while (t.cat == SN && (t.codigo == OP_IGUAL || t.codigo == OP_DIFERENTE || t.codigo == OP_MAIOR || t.codigo == OP_MENOR || t.codigo == OP_MAIOR_IGUAL || t.codigo == OP_MENOR_IGUAL)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_aditiva();
    }
    diminui_ident(); printf("%s</Expr_relacional>\n", TABS);
}

void Expr_aditiva() {
    printf("%s<Expr_aditiva>\n", TABS); aumenta_ident();
    Expr_multiplicativa();
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        print_folha(t); consome(SN, t.codigo);
        Expr_multiplicativa();
    }
    diminui_ident(); printf("%s</Expr_aditiva>\n", TABS);
}

void Expr_multiplicativa() {
    printf("%s<Expr_multiplicativa>\n", TABS); aumenta_ident();
    Fator();
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        print_folha(t); consome(SN, t.codigo);
        Fator();
    }
    diminui_ident(); printf("%s</Expr_multiplicativa>\n", TABS);
}

void Fator() {
    printf("%s<Fator>\n", TABS); aumenta_ident();
    if (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO || t.codigo == OP_NOT)) {
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
            if (!(t.cat == SN && t.codigo == FECHA_PARENTESES)) {
                Expr();
                while (t.cat == SN && t.codigo == VIRGULA) {
                    print_folha(t); consome(SN, VIRGULA);
                    Expr();
                }
            }
            print_folha(t); consome(SN, FECHA_PARENTESES);
        }
    } else if (t.cat == CT_INT || t.cat == CT_REAL || t.cat == CT_CHAR) {
        print_folha(t); consome(t.cat, 0);
    } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        print_folha(t); consome(SN, ABRE_PARENTESES);
        Expr();
        print_folha(t); consome(SN, FECHA_PARENTESES);
    } else {
        error("Fator mal formado em uma expressao (esperado ID, constante ou '(' ).");
    }
    diminui_ident(); printf("%s</Fator>\n", TABS);
}