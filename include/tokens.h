#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_ID,
    TOKEN_INTCON,
    TOKEN_REALCON,
    TOKEN_CHARCON,
    TOKEN_STRINGCON,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_KEYWORD,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
    int column;
} Token;

#endif