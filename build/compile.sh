#!/bin/bash

echo "🔧 Compilando analisador léxico com GCC..."

gcc lex/anaLex.c main.c -o anaLexTest.exe -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: anaLexTest.exe"
    echo "▶️ Executando anaLexTest.exe..."
    ../anaLexTest.exe
else
    echo "❌ Erro na compilação!"
fi
