#ifndef KEYWORDS_H
#define KEYWORDS_H

int is_keyword(const char *lexeme);  // Verifica se é palavra-chave
void init_keywords();                // Inicializa a tabela
void free_keywords();                // Libera memória da tabela

#endif
