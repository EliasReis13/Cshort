#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/keywords.h"

#define TABLE_SIZE 31 // Número primo para evitar colisões

typedef struct Entry {
    char *keyword;
    struct Entry *next;
} Entry;

static Entry *table[TABLE_SIZE];

static unsigned hash(const char *str) {
    unsigned h = 0;
    for(; *str; str++) {
        h = (h << 4) + *str;
        unsigned g = h & 0xF0000000;
        if (g) h ^= g >> 24;
        h &= ~g;
    }
    return h % TABLE_SIZE;
}

void init_keywords() {
    const char *keywords[] = {
        "int", "char", "float", "bool", "void", 
        "if", "else", "for", "while", "return"
    };
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        unsigned idx = hash(keywords[i]);
        Entry *e = malloc(sizeof(Entry));
        e->keyword = strdup(keywords[i]);
        e->next = table[idx];
        table[idx] = e;
    }
}

int is_keyword(const char *lexeme) {
    unsigned idx = hash(lexeme);
    Entry *e = table[idx];
    while (e) {
        if (strcmp(e->keyword, lexeme) == 0) {
            return 1; 
        }
        e = e->next;
    }
    return 0;
}