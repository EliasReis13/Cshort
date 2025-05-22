#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

void init_lexer(const char *filename);
Token next_token(void);
void close_lexer(void);

#endif
