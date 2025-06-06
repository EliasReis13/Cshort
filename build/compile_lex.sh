#!/bin/bash

echo "🔧 Compilando analisador léxico..."

# Garante que a pasta bin/ exista
mkdir -p bin

# Compila o léxico isolado
gcc src/lex/anaLex.c src/lex/main.c -o bin/cshort.exe -Wall -Wextra

# Verifica se compilou com sucesso
if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.exe"
    echo "▶️ Executando bin/cshort.exe com teste padrão..."

    # Executa com um arquivo de teste padrão
    ./bin/cshort.exe test/teste_erro1.cshort
else
    echo "❌ Erro na compilação léxica!"
fi
