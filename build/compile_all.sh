#!/bin/bash
echo "🔧 Compilando o compilador completo..."
mkdir -p bin

# Comando GCC com os caminhos corretos para todos os arquivos .c
# e a flag -I src para encontrar os cabeçalhos.
gcc src/main.c \
    src/lex/anaLex.c \
    src/sint/anaSint.c \
    src/tabela/tabelaSimbolos.c \
    src/gerador/geradorCodigo.c \
    -o bin/cshort.out -I src

if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.out"
else
    echo "❌ Erro na compilação!"
fi