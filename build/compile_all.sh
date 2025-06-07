#!/bin/bash

echo "🔧 Compilando o compilador completo (por enquanto, só léxico)..."

# Garante que a pasta bin/ exista
mkdir -p bin out

# Compila o analisador léxico + main central
gcc src/lex/anaLex.c src/main.c -o bin/cshort.exe -Wall -Wextra

# Verifica se compilou com sucesso
if [ $? -eq 0 ]; then
    echo "✅ Compilado com sucesso: bin/cshort.exe"
    echo "▶️ Executando cshort.exe com arquivo de teste..."

    # Executa com um arquivo de teste padrão
    ./bin/cshort.exe test/teste_erro1.cshort
else
    echo "❌ Erro na compilação!"
fi
