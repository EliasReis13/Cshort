@echo off
echo Compilando analisador léxico com GCC...

gcc lex\anaLex.c main.c -o anaLexTest.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo Compilado com sucesso: anaLexTest.exe
    echo Executando anaLexTest.exe com arquivo de teste...
    anaLexTest.exe test\teste1.cshort
) else (
    echo ❌ Erro na compilação!
)
