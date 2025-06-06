@echo off
echo Compilando o compilador completo (por enquanto, só léxico)...

REM Cria a pasta bin se não existir
if not exist bin mkdir bin
if not exist out mkdir out

REM Compila o analisador léxico + main geral (caminhos relativos à raiz)
gcc src\lex\anaLex.c src\main.c -o bin\cshort.exe -Wall -Wextra

if %errorlevel% equ 0 (
    echo ✅ Compilado com sucesso: bin\cshort.exe
    echo ▶️ Executando bin\cshort.exe com arquivo de teste...
    bin\cshort.exe test\teste_erro1.cshort
) else (
    echo ❌ Erro na compilação!
)
