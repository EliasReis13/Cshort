@echo off
echo Compilando analisador léxico isolado...

REM Cria a pasta bin se não existir
if not exist bin mkdir bin
if not exist out mkdir out

REM Compila o léxico com o main da pasta lex
gcc src\lex\anaLex.c src\lex\main.c -o bin\cshort.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo Compilado com sucesso: bin\cshort.exe
    echo Executando bin\cshort.exe com arquivo de teste...
    bin\cshort.exe test\teste1.cshort
) else (
    echo ❌ Erro na compilação léxica!
)
