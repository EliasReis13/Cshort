# 🔍 Compilador da Linguagem Cshort

Este projeto implementa um **compilador** para a linguagem fictícia **Cshort**, conforme a especificação presente no documento `Especificação Cshort-v1.0.pdf`.

Atualmente, estão implementadas as fases de:
- ✅ Análise Léxica
- ✅ Análise Sintática
- 🛠️ Em desenvolvimento: Análise Semântica

---

## 📚 Sobre a Linguagem Cshort

A linguagem **Cshort** é uma linguagem estruturada inspirada em C. Sua gramática é descrita em BNF estendida, e ela oferece suporte a:

- Tipos primitivos: `int`, `float`, `char`, `bool`, `string`
- Declarações de variáveis, vetores e funções
- Controle de fluxo: `if`, `else`, `while`, `for`
- Operadores aritméticos, relacionais, lógicos e de atribuição
- Literais: inteiros, reais, caracteres e strings
- Comentários multilinha: `/* ... */`

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
│   ├── main.c              # Main do compilador completo
│   ├── lex/                # Analisador léxico
│   │   ├── anaLex.c
│   │   ├── anaLex.h
│   │   └── main.c
│   └── sint/               # Analisador sintático
│       ├── anaSint.c
│       ├── anaSint.h
│       └── main.c
├── test/                   # Casos de teste da linguagem
│   ├── teste_1.cshort
│   ├── teste_2.cshort
│   ├── teste_com_erros.cshort
│   └── teste_sem_erros.cshort
├── .gitignore
└── README.md

```

---


## 🛠 Componentes do Projeto

| Caminho                          | Descrição                                       |
|----------------------------------|-------------------------------------------------|
| `src/lex/anaLex.c`               | Implementação do analisador léxico             |
| `src/lex/anaLex.h`               | Definições de tokens e protótipos              |
| `src/lex/main.c`                 | Main de teste apenas do léxico                 |
| `src/sint/anaSint.c`             | Implementação do analisador sintático          |
| `src/sint/anaSint.h`             | Cabeçalhos do analisador sintático             |
| `src/sint/main.c`                | Teste isolado da análise sintática             |
| `src/main.c`                     | Main geral do compilador                       |
| `build/compile_*.sh / .bat`      | Scripts de build para Linux e Windows          |
| `test/*.cshort`                  | Casos de teste válidos e com erros léxicos     |
| `out/tokens.txt`                 | Resultado da análise léxica                    |
| `AFD/`                           | Representações gráficas do autômato            |
| `Doc/Especificação Cshort...`    | Documento oficial da linguagem                 |
| `bin/`                           | Executáveis gerados pelos scripts     



## ▶️ Como Compilar e Executar

### 🐧 Linux/macOS

#### Compilar e executar o compilador completo:
```bash
./build/compile_all.sh
```

#### Compilar e executar apenas o analisador léxico isolado:
```bash
./build/compile_lex.sh
```

### 🪟 Windows

#### Compilar e executar o compilador completo:

```cmd
build\compile_all.bat
```

#### Compilar e executar apenas o analisador léxico isolado:

```cmd
build\compile_lex.bat
```

## 🧪 Exemplo de Saída

Para a entrada em Cshort:

```c
int main() {
    char c = 'a';
    int x = 10;
}
```
A saída será semalhante a:

```c
<ID, main>
<ABRE_PARENTESES, (>
<FECHA_PARENTESES, )>
<ABRE_CHAVE, {>
<PR_CHAR, 5>
<ID, c>
<OP_ATRIBUICAO, 5>
<CT_CHAR, 'a'>
<END_EXPR, ;>
<PR_INT, 3>
<ID, x>
<OP_ATRIBUICAO, 5>
<CT_INT, 10>
<END_EXPR, ;>
<FECHA_CHAVE, }>
<EOF>
```

## ✅ Componentes

### Analisador Léxico

- Caminho: `src/lex/anaLex.c`
- Entrada: arquivo `.cshort`
- Saída: tokens categorizados (ID, palavras reservadas, operadores, literais...)

### Analisador Sintático

- Caminho: src/sint/anaSint.c ou anaSint_expandido.c
- Entrada: sequência de tokens
- Saída: árvore sintática com identação

## ⚙️ Funcionalidades do Analisador

- Reconhecimento de:
  - Identificadores e palavras reservadas
  - Literais numéricos, `char` e `string`
  - Operadores e delimitadores
  - Comentários multilinha (`/* ... */`)
- Tratamento de erros léxicos com indicação da linha
- Geração de tokens com suas categorias e valores


---


## 👨‍💻 Autor

Este projeto foi desenvolvido como parte da disciplina **Fundamentos de Compiladores**, no curso de **Sistemas de Informação da UNEB**.
