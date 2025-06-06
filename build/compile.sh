#!/bin/bash

echo "🔧 Compilando analisador léxico com GCC..."

mkdir -p bin

gcc src/lex/anaLex.c src/main.c -o bin/anaLexTest.exe -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/anaLexTest.exe"
    echo "▶️ Executando bin/anaLexTest.exe com teste padrão..."
    ./bin/anaLexTest.exe test/teste1.cshort
else
    echo "❌ Erro na compilação!"
fi
