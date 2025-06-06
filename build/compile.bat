@echo off
echo 🔧 Compilando analisador léxico com GCC...

if not exist bin (
    mkdir bin
)

gcc src\lex\anaLex.c src\main.c -o bin\anaLexTest.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo ✅ Compilado com sucesso: bin\anaLexTest.exe
    echo ▶️ Executando bin\anaLexTest.exe com teste padrão...
    bin\anaLexTest.exe test\teste1.cshort
) else (
    echo ❌ Erro na compilação!
)
