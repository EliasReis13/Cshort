#!/bin/bash
echo "🔧 Compilando o compilador completo..."
mkdir -p bin

gcc src/main.c \
    src/lex/anaLex.c \
    src/sint/anaSint.c \
    src/tabela/tabelaSimbolos.c \
    -o bin/cshort.out -I src

if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.out"
else
    echo "❌ Erro na compilação!"
fi