# 🔍 Compilador da Linguagem Cshort

Este projeto implementa um **compilador** para a linguagem fictícia **Cshort**, conforme especificação descrita no documento `Especificação Cshort-v1.0.pdf`.

Atualmente, está implementada a **fase de análise léxica**, responsável por identificar os tokens válidos em um programa escrito em Cshort.

---

## 📚 Sobre a Linguagem Cshort

A linguagem Cshort é uma linguagem de programação estruturada, inspirada em C. Sua especificação é baseada em uma gramática BNF estendida, com suporte a:

- Tipos básicos: `int`, `float`, `char`, `bool`, `string`
- Variáveis, arrays e funções
- Estruturas de controle: `if`, `else`, `while`, `for`
- Operadores aritméticos, relacionais, lógicos e de atribuição
- Literais: inteiros, reais, caracteres e strings
- Comentários multilinha estilo C: `/* comentário */`

---

## 📁 Estrutura de Diretórios

```plaintext
Cshort/
├── bin/                     # Executáveis gerados
│   ├── cshort.exe           # Compilador completo
│   └── anaLexTest.exe       # Analisador léxico isolado
├── build/                   # Scripts de compilação
│   ├── compile_all.bat      # Windows - compilador completo
│   ├── compile_lex.bat      # Windows - apenas léxico
│   ├── compile_all.sh       # Linux/macOS - compilador completo
│   └── compile_lex.sh       # Linux/macOS - apenas léxico
├── src/                     # Código-fonte
│   ├── main.c               # Ponto de entrada principal
│   └── lex/                 # Analisador léxico
│       ├── anaLex.c
│       ├── anaLex.h
│       └── main.c           # Main de teste para o léxico
├── test/                    # Casos de teste da linguagem
│   ├── teste1.cshort
│   ├── teste2.cshort
│   └── ... outros testes ...
├── tokens.txt               # Saída gerada com os tokens reconhecidos
├── README.md
└── Especificação Cshort-v1.0.pdf

```

---


## 🛠 Componentes

| Caminho                         | Descrição |
|--------------------------------|-----------|
| `src/lex/anaLex.c`             | Implementação do analisador léxico. |
| `src/lex/anaLex.h`             | Definições de tokens e protótipos. |
| `src/main.c`                   | Função principal do compilador. |
| `test/`                        | Casos de teste `.cshort`. |
| `build/compile.sh`             | Script de compilação para Linux/macOS. |
| `build/compile.bat`            | Script de compilação para Windows. |
| `out/tokens.txt`               | Saída gerada com os tokens reconhecidos. |
| `bin/anaLexTest.exe`           | Executável gerado. |
| `Especificação Cshort-v1.0.pdf`| Documento oficial da linguagem. 

## ▶️ Como Compilar e Executar

### Linux/macOS
```bash
chmod +x compile.sh
./compile.sh
```

### 🪟 Windows

Execute o arquivo `compile.bat` com um duplo clique ou via terminal (Prompt de Comando):

```cmd
compile.bat
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
