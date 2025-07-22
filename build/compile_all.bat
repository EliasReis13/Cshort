@echo off
echo Compilando o compilador completo...

rem Cria a pasta 'bin' se ela não existir e suprime a mensagem de erro se já existir.
mkdir bin 2>nul

rem Comando GCC com o nome do arquivo corrigido (geradorCodigo.c)
gcc -o bin/cshort.exe src/main.c src/lex/anaLex.c src/sint/anaSint.c src/tabela/tabelaSimbolos.c src/gerador/geradorCodigo.c -I src

if %errorlevel% == 0 (
    echo Compilado com sucesso: bin/cshort.exe
) else (
    echo Erro na compilacao!
)