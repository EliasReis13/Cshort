@echo off
echo Compilando o compilador completo (lexico e sintatico)

if not exist bin mkdir bin
if not exist out mkdir out

REM Compila todos os .c juntos. 
gcc src\lex\anaLex.c src\sint\anaSint.c src\tabela\tabelaSimbolos.c src\main.c -o bin\cshort.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo Compilado com sucesso: bin\cshort.exe
    echo Executando bin\cshort.exe com arquivo de teste...
    bin\cshort.exe test\teste_com_erros.cshort
) else (
    echo Erro na compilacao!
)