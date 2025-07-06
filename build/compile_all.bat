@echo off
echo Compilando o compilador completo (lexico, sintatico e semantico)

if not exist bin mkdir bin
if not exist out mkdir out

REM Compila todos os .c juntos. 
REM O caminho para main.c foi corrigido para "src\main.c"
gcc src\lex\anaLex.c src\sint\anaSint.c src\tabela\tabelaSimbolos.c src\main.c -o bin\cshort.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo Compilado com sucesso: bin\cshort.exe
    echo Executando bin\cshort.exe com arquivo de teste...
    REM O caminho para o arquivo de teste pode precisar de ajuste
    bin\cshort.exe test\teste_sem_erros.cshort
) else (
    echo Erro na compilacao!
)