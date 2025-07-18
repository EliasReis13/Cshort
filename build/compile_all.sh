#!/bin/bash

echo "🔧 Compilando o compilador completo (léxico e sintático)"

# Cria as pastas de saída se não existirem
mkdir -p bin
mkdir -p out

gcc src/lex/anaLex.c src/sint/anaSint.c src/tabela/tabelaSimbolos.c src/main.c -o bin/cshort.out -Wall -Wextra

# Verifica se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.out"
    echo "▶️ Executando cshort.out com arquivo de teste..."

    # Executa o compilador com um arquivo de teste
    ./bin/cshort.out test/teste_sem_erros.cshort
 
else
    echo "❌ Erro na compilação!"
fi
