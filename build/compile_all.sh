#!/bin/bash

echo "🔧 Compilando o compilador completo (léxico, sintático, semântico e gerador)..."

mkdir -p bin

gcc src/main.c \
    src/lex/anaLex.c \
    src/sint/anaSint.c \
    src/tabela/tabelaSimbolos.c \
    src/gerador/geradorCodigo.c \
    -o bin/cshort.out -I src

# Verifica o código de saída do último comando ($?). 0 significa sucesso.
if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.out"
else
    echo "❌ Erro na compilação!"
fi