@echo off
echo Compilando analisador lwxico com GCC...

gcc anaLex.c main.c -o anaLexTest.exe -Wall -Wextra

IF %ERRORLEVEL% EQU 0 (
    echo Compilado com sucesso: anaLexTest.exe
    echo Executando anaLexTest.exe...
    anaLexTest.exe
) ELSE (
    echo ❌ Erro na compilação!
)
pause
