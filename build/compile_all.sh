#!/bin/bash

echo "🔧 Compilando o compilador completo (léxico e sintático) "

# Cria pastas se não existirem
mkdir -p bin
mkdir -p out

# Compila usando gcc
gcc src/lex/anaLex.c src/sint/anaSint.c src/sint/main.c -o bin/cshort.out -Wall -Wextra

# Verifica se compilou com sucesso
if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.exe"
    echo "▶️ Executando cshort.exe com arquivo de teste..."

    # Executa com um arquivo de teste padrão
    ./bin/cshort.exe test/teste_erro3.cshort
else
    echo "❌ Erro na compilação!"
fi
