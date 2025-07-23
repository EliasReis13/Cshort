# 🔍 Compilador da Linguagem Cshort

Este projeto implementa um compilador para a linguagem fictícia **Cshort**, conforme as especificações presentes no documento `Doc/Especificação Cshort-v1.0.pdf`.

O compilador realiza a análise completa do código-fonte (front-end) e gera código intermediário para uma máquina de pilha (início do back-end).

Atualmente, estão implementadas as fases de:
- ✅ **Análise Léxica**: Reconhecimento de todos os tokens da linguagem.
- ✅ **Análise Sintática**: Validação da estrutura gramatical do código.
- ✅ **Tabela de Símbolos**: Gerenciamento de escopo (global/local) e de identificadores.
- ✅ **Análise Semântica (DDS)**: Verificação de declarações e compatibilidade de tipos em expressões e comandos.
- ✅ **Geração de Código Intermediário**: Tradução de comandos e expressões para código de máquina de pilha.

---

## 📚 Sobre a Linguagem Cshort

A linguagem **Cshort** é uma linguagem estruturada inspirada em C. Sua gramática é descrita em BNF estendida, e ela oferece suporte a:

- **Tipos primitivos**: `int`, `float`, `char`, `bool`, `void`.
- **Declarações**: Variáveis globais/locais, protótipos e definições de funções.
- **Estruturas de Controle**: `if-else`, `while`, `for`.
- **Operadores**: Aritméticos (`+`, `-`, `*`, `/`), relacionais (`>`, `==`, etc.) e lógicos (`&&`, `||`).
- **Comentários**: `/* ... */` e `// ...` (se implementado no léxico).

---

## 📁 Estrutura de Diretórios

```plaintext
Cshort/
├── .vscode/                # Configurações do VSCode
│   └── tasks.json
├── AFD/                    # Autômato finito determinístico
│   ├── AFD.jff
│   ├── AFD.jpg
│   └── JFLAP7.1.jar
├── bin/                    # Executável gerado
│   └── cshort.exe
├── build/                  # Scripts de compilação
│   ├── compile_all.bat
│   ├── compile_all.sh
│   ├── compile_lex.bat
│   └── compile_lex.sh
├── Doc/                    # Documentação do projeto
│   ├── Especificação Cshort-v1.0.pdf
│   └── Implementação de analisador.pdf
├── out/                    # Saídas do compilador
│   └── tokens.txt
├── src/                    # Código-fonte
│   ├── gerador/            # Módulo do Gerador de Código
│   │   ├── geradorCodigo.c
│   │   └── geradorCodigo.h
│   ├── main.c              # Main do compilador completo
│   ├── lex/                # Analisador léxico
│   │   ├── anaLex.c
│   │   ├── anaLex.h
│   │   └── main.c
│   ├── sint/               # Analisador sintático
│   │   ├── anaSint.c
│   │   ├── anaSint.h
│   │   └── main.c
│   └── tabela/             # Módulo de tabela de símbolos
│       ├── tabelaSimbolos.c
│       └── tabelaSimbolos.h
├── test/                   # Casos de teste da linguagem
│   ├── teste_com_erros.cshort
│   └── teste_sem_erros.cshort
├── .gitignore
└── README.md

```

---


## 🛠 Componentes do Projeto

| Caminho                          | Descrição                                       |
|----------------------------------|-------------------------------------------------|
| `src/gerador/geradorCodigo.c`    | Gerador de código                               |
| `src/gerador/geradorCodigo.h`    | Cabeçalho do Gerador                            |
| `src/lex/anaLex.c`               | Implementação do analisador léxico              |
| `src/lex/anaLex.h`               | Definições de tokens e protótipos               |
| `src/lex/main.c`                 | Main de teste apenas do léxico                  |
| `src/sint/anaSint.c`             | Implementação do analisador sintático           |
| `src/sint/anaSint.h`             | Cabeçalhos do analisador sintático              |
| `src/sint/main.c`                | Teste isolado da análise sintática              |
| `src/tabela/tabelaSimbolos.c`    | Implementação da tabela de símbolos             | 
| `src/tabela/tabelaSimbolos.h`    | Interface da tabela de símbolos                 |
| `src/main.c`                     | Main geral do compilador                        |
| `build/compile_*.sh / .bat`      | Scripts de build para Linux e Windows           |
| `test/*.cshort`                  | Casos de teste válidos e com erros léxicos      |
| `out/tokens.txt`                 | Resultado da análise léxica                     |
| `AFD/`                           | Representações gráficas do autômato             |
| `Doc/`                           | Documentação da linguagem                       |
| `bin/`                           | Executáveis gerados pelos scripts               |
| `.vscode/tasks.json`             | 	Configuração de tarefas para o VSCode          |


## ⚙️ Funcionalidades Implementadas

### Análise Léxica (`src/lex/`)
- Reconhece todos os tokens da linguagem, incluindo palavras-chave, identificadores, constantes numéricas, caracteres, strings e operadores.
- Trata corretamente comentários de bloco e conta as linhas para reportar erros com precisão.

### Tabela de Símbolos (`src/tabela/`)
- Implementada como uma pilha para gerenciar escopos aninhados (`global` e `local`) de forma eficiente.
- Armazena informações detalhadas sobre cada símbolo, como seu nome, tipo, categoria (variável, função) e escopo.
- Fornece a base para toda a análise semântica.

### Análise Sintática e Semântica (`src/sint/`)
- Utiliza um parser de descida recursiva para validar a estrutura do código Cshort.
- **Definição Dirigida pela Sintaxe (DDS) para Análise Semântica:**
  - **Gerenciamento de Escopo:** Utiliza a tabela de símbolos para gerenciar escopos `global` e `local`, permitindo o sombreamento de variáveis.
  - **Verificação de Declarações:** Garante que identificadores não sejam redeclarados no mesmo escopo e que sejam declarados antes do uso.
  - **Checagem de Tipos:** Valida a compatibilidade de tipos em:
    - **Expressões Aritméticas:** Verifica se operandos de `+`, `-`, `*`, `/` são compatíveis (ex: `int` com `char`, `float` com `float`).
    - **Expressões Relacionais/Lógicas:** Verifica os operandos e garante que o resultado seja `bool`.
    - **Comandos Condicionais:** Assegura que as condições em `if`, `while` e `for` resultem em um tipo `bool` (ou `int`).
    - **Atribuições:** Verifica se o tipo da expressão do lado direito é compatível com a variável do lado esquerdo.

### Geração de Código Intermediário(`src/gerador/`)
- O compilador traduz o código Cshort para uma representação de baixo nível para uma **Máquina de Pilha**.
- **Esquemas de Tradução Implementados:**
  - **Expressões Aritméticas:** Gera instruções LOAD (para variáveis), PUSH (para constantes) e ADD, SUB, MUL, DIV para os cálculos.  
  - **Expressões Condicionais:** Gera instruções de comparação como GT (maior que), LT (menor que), EQ (igual), etc.
  - **Estruturas de Controle:** Gera a lógica de desvio com rótulos e saltos (JUMP, JUMP_FALSE) para os comandos if-else e while.
- O código gerado é salvo em um arquivo de saída (ex: out/codigo_gerado.maq) para posterior simulação ou compilação.


## ▶️ Como Compilar e Executar

Utilize os scripts na pasta `build/` a partir da raiz do projeto.


### 🐧 Linux/macOS

```bash
# Para compilar
./build/compile_all.sh

# Para executar com um arquivo de teste
./bin/cshort.out test/teste_sem_erros.cshort
```

### 🪟 Windows


```cmd
REM Para compilar
.\build\compile_all.bat

REM Para executar com um arquivo de teste
bin\cshort.exe test\teste_sem_erros.cshort
```

## ✅ Componentes

### Analisador Léxico

- Caminho: `src/lex/anaLex.c`
- Entrada: arquivo `.cshort`
- Saída: tokens categorizados (ID, palavras reservadas, operadores, literais...)

### Analisador Sintático

- Caminho: src/sint/anaSint.c 
- Entrada: sequência de tokens
- Saída: árvore sintática com identação

## ⚙️ Funcionalidades 

### 🔠 Analisador Léxico

- Reconhecimento de:
  - Identificadores e palavras reservadas
  - Literais (`int`, `float`, `char`, `string`)
  - Operadores e delimitadores
  - Comentários multilinha (`/* ... */`)
- Tratamento de erros léxicos com indicação da linha
- Saída com categorias e valores dos tokens

### 🧩 Analisador Sintático

- Leitura da sequência de tokens
- Construção da árvore sintática (indentada)
- Detecção de erros de estrutura

### 🗂️ Tabela de Símbolos 
- Módulo separado
- Interface para registrar identificadores e escopos 
---


## 👨‍💻 Autor

Este projeto foi desenvolvido como parte da disciplina **Fundamentos de Compiladores**, no curso de **Sistemas de Informação da UNEB**.
