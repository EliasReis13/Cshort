// Analisador Sintático Expandido para linguagem Cshort
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "anaSint.h"
#include "../lex/anaLex.h"

// --- Variáveis Globais ---
FILE *fd;
TOKEN t, tLookahead; // Token atual e token de lookahead
char TABS[200] = "";
bool modoPanico = false;
bool houveErroSintatico = false;

// --- Protótipos de Funções Auxiliares Locais ---
void nextToken();
void error(char msg[]);
void aumenta_ident();
void diminui_ident();
void print_folha(TOKEN tk);
void sincroniza();

// --- Implementação das Funções Auxiliares ---

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

void nextToken() {
    t = tLookahead;
    if (t.cat != END_FILE) {
        tLookahead = AnaLex(fd);
    }
}

void sincroniza() {
    bool pontoEncontrado = false; // Flag para controlar a saída

    while (t.cat != END_FILE && !pontoEncontrado) { // Condição de parada na flag
        if (t.cat == END_EXPRESSION) {
            nextToken();
            pontoEncontrado = true;
        }
        else if (t.cat == SN && t.codigo == FECHA_CHAVE) {
            pontoEncontrado = true;
        }
        else if (t.cat == RESERVED_WORD) {
            switch(t.codigo) {
                case PR_IF:
                case PR_WHILE:
                case PR_FOR:
                case PR_RETURN:
                case PR_INT:
                case PR_FLOAT:
                case PR_CHAR:
                case PR_STRING:
                    pontoEncontrado = true; // Apenas define a flag, não pula
                    break; // Sai do switch
                default:
                    nextToken(); // Consome token se não for um ponto de sinc.
            }
        } else {
            nextToken(); // Consome qualquer outro token
        }
    }
    modoPanico = false; // Desativa o modo pânico após sincronizar
} // <-- Este é o final correto da função

void consome(int categoria, int codigo) {
    if (modoPanico) return; 

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

// --- Implementação das Funções do Parser ---

int Tipo() {
    if (t.cat == RESERVED_WORD) {
        switch (t.codigo) {
            case PR_INT:
            case PR_FLOAT:
            case PR_CHAR:
            case PR_STRING:
            case PR_VOID: // Adicionado void como tipo válido aqui
                return 1;
        }
    }
    return 0;
}

void Prog() {
    printf("<Prog>\n"); 
    aumenta_ident();
    
    // Inicialização correta do sistema de lookahead
    tLookahead = AnaLex(fd);
    nextToken();
    
    while (t.cat != END_FILE) {
        DECL_SINALIZADOR flag = Decl();

        
        if (flag == DECL_FUNC) {
            corpo_func();
        } 

    }
    
    diminui_ident(); 
    printf("</Prog>\n");
}

void corpo_func() {
    printf("%s<corpo_func>\n", TABS);
    aumenta_ident();
    Cmd_bloco();
    diminui_ident();
    printf("%s</corpo_func>\n", TABS);
}

DECL_SINALIZADOR Decl() {
    DECL_SINALIZADOR declFlag = NO_DECL;

    // Garante que a declaração começa com um tipo
    if (!Tipo()) {
        error("Esperado um tipo (int, float, char, etc) no inicio da declaracao.");
        nextToken(); // Consome o token inválido
        sincroniza();
        return NO_DECL;
    }
    
    int tipo_declarado = t.codigo; 
    nextToken(); // Consome o tipo (ex: 'int')

    // Garante que um identificador (nome) venha após o tipo
    if (t.cat != ID) {
        error("Identificador esperado apos o tipo.");
        sincroniza(); // Sincroniza para tentar se recuperar
        return NO_DECL;
    }
    
    // Olhamos um token à frente para ver se é uma função
    if (tLookahead.cat == SN && tLookahead.codigo == ABRE_PARENTESES) {
        // --- Início da nova lógica para funções ---

        nextToken(); // Consome o ID da função. Agora 't' é o token '('
        nextToken(); // Consome o '('. Agora 't' é o início dos parâmetros
        
        Tipos_param(); // Analisa a lista de parâmetros
        
        consome(SN, FECHA_PARENTESES); // Espera e consome o ')'
        
        // Verifica o que vem DEPOIS do ')' para decidir
        if (t.cat == SN && t.codigo == ABRE_CHAVE) {
            // Se for um '{', então é uma DEFINIÇÃO de função.
            declFlag = DECL_FUNC;
            // Importante: Não consumimos o '{'. A função corpo_func() fará isso.
        } else {
            // Se não for '{', asumimos que é um PROTÓTIPO e deve terminar com ';'.
            declFlag = DECL_PROT;
            consome(END_EXPRESSION, 0); // Espera e consome o ';'
        }
        // --- Fim da nova lógica para funções ---

    } else {
        // Se não for uma função, é uma declaração de variável.
        declFlag = DECL_VAR;
        if (tipo_declarado == PR_VOID) {
            error("Variaveis nao podem ser do tipo 'void'.");
        }

        // Loop para tratar múltiplas declarações de variáveis (ex: int a, b, c;)
        while(true) {
            if (t.cat != ID) {
                error("Identificador de variavel esperado.");
                break; // Sai do loop se algo der errado
            }
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
                break; // Se não for vírgula, a lista de declarações terminou
            }
        }
        consome(END_EXPRESSION, 0); // Consome o ';' no final da linha
    }
    
    return declFlag;
}

void Tipos_param() {
    // Primeiro, verifica o caso de não haver parâmetros, ex: (void)
    if (t.cat == RESERVED_WORD && t.codigo == PR_VOID) {
        // Olhamos à frente para ter certeza de que é só 'void' e ')'
        if (tLookahead.cat == SN && tLookahead.codigo == FECHA_PARENTESES) {
            consome(RESERVED_WORD, PR_VOID); // Consome 'void'
            return; // Termina a análise de parâmetros
        }
    }

    // Se não for o caso especial de (void), ou se houver parâmetros
    if (Tipo()) { // Verifica se há um tipo válido
        while (true) {
            if (!Tipo()) break; // Para se não encontrar mais tipos

            consome(t.cat, t.codigo); // Consome o tipo (ex: 'int')

            // --- CORREÇÃO PRINCIPAL ---
            // O identificador do parâmetro é OPCIONAL, então só consumimos se ele existir.
            if (t.cat == ID) {
                consome(ID, 0); // Consome o nome do parâmetro (ex: 'n')
            }

            // Opcional: tratar arrays em parâmetros
            if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
                consome(SN, ABRE_COLCHETE);
                consome(SN, FECHA_COLCHETE);
            }

            // Se não houver uma vírgula, a lista de parâmetros acabou
            if (t.cat != SN || t.codigo != VIRGULA) {
                break;
            }
            consome(SN, VIRGULA); // Consome a vírgula para o próximo parâmetro
        }
    }

}

void Cmd() {
    if (Tipo()) { // Uma declaração dentro de um bloco é um comando
        Decl();
        
    }
    else if (t.cat == RESERVED_WORD && t.codigo == PR_IF) Cmd_if();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_WHILE) Cmd_while();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_FOR) Cmd_for();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_RETURN) Cmd_return();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_BREAK) Cmd_break();
    else if (t.cat == RESERVED_WORD && t.codigo == PR_CONTINUE) Cmd_continue();
    else if (t.cat == SN && t.codigo == ABRE_CHAVE) Cmd_bloco();
    else {
        Expr();
        consome(END_EXPRESSION, 0);
    }
}

void Cmd_if() {
    consome(RESERVED_WORD, PR_IF);
    consome(SN, ABRE_PARENTESES);
    Expr();
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
    Expr();
    consome(SN, FECHA_PARENTESES);
    Cmd();
}

void Cmd_for() {
    consome(RESERVED_WORD, PR_FOR);
    consome(SN, ABRE_PARENTESES);
    if(t.cat != END_EXPRESSION) Expr(); 
    consome(END_EXPRESSION, 0);
    if(t.cat != END_EXPRESSION) Expr(); 
    consome(END_EXPRESSION, 0);
    if(t.cat != SN || t.codigo != FECHA_PARENTESES) Expr();
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
        if (modoPanico) {
            sincroniza();
            if (t.cat == SN && t.codigo == FECHA_CHAVE) break;
        }
        Cmd();
    }
    consome(SN, FECHA_CHAVE);
}

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

void Expr_relacional() {
    Expr_aditiva();
    while (t.cat == SN && (t.codigo == OP_IGUAL || t.codigo == OP_DIFERENTE || t.codigo == OP_MAIOR || t.codigo == OP_MAIOR_IGUAL || t.codigo == OP_MENOR || t.codigo == OP_MENOR_IGUAL)) {
        consome(SN, t.codigo);
        Expr_aditiva();
    }
}

void Expr_aditiva() {
    Expr_multiplicativa();
    while (t.cat == SN && (t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        consome(SN, t.codigo);
        Expr_multiplicativa();
    }
}

void Expr_multiplicativa() {
    Fator();
    while (t.cat == SN && (t.codigo == OP_MULTIPLICACAO || t.codigo == OP_DIVISAO)) {
        consome(SN, t.codigo);
        Fator();
    }
}

void Fator() {
    if (t.cat == SN && (t.codigo == OP_NOT || t.codigo == OP_SOMA || t.codigo == OP_SUBTRACAO)) {
        consome(SN, t.codigo);
        Fator();
    } else if (t.cat == ID) {
        consome(ID, 0);
        if (t.cat == SN && t.codigo == ABRE_COLCHETE) {
            consome(SN, ABRE_COLCHETE);
            Expr();
            consome(SN, FECHA_COLCHETE);
        } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
            consome(SN, ABRE_PARENTESES);
            if (t.cat != SN || t.codigo != FECHA_PARENTESES) {
                Expr();
                while (t.cat == SN && t.codigo == VIRGULA) {
                    consome(SN, VIRGULA);
                    Expr();
                }
            }
            consome(SN, FECHA_PARENTESES);
        }
    } else if (t.cat == CT_INT || t.cat == CT_REAL || t.cat == CT_CHAR || t.cat == CT_STRING) {
        consome(t.cat, 0);
    } else if (t.cat == SN && t.codigo == ABRE_PARENTESES) {
        consome(SN, ABRE_PARENTESES);
        Expr();
        consome(SN, FECHA_PARENTESES);
    } else {
        error("Expressao mal formada no fator.");
    }
}