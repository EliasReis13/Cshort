# 🔍 Analisador Léxico da Linguagem Cshort

Este projeto implementa um **analisador léxico** para a linguagem fictícia **Cshort**, conforme especificação descrita no documento `Especificação Cshort-v1.0.pdf`.

## 📚 Sobre a Linguagem Cshort

A linguagem Cshort é uma linguagem de programação estruturada inspirada em C. Sua gramática é descrita em BNF estendida, com suporte a:

- Tipos básicos: `int`, `float`, `char`, `bool`
- Variáveis, arrays e funções
- Estruturas de controle: `if`, `else`, `while`, `for`
- Operadores: aritméticos, relacionais, lógicos, atribuição
- Literais: inteiros, reais, caracteres, strings
- Comentários estilo C (`/* comentário */`)

## 🛠 Arquivos do Projeto

| Arquivo           | Descrição |
|------------------|-----------|
| `anaLex.c`       | Implementação do analisador léxico (tokenização). |
| `anaLex.h`       | Declarações de tipos, tokens e protótipos. |
| `main.c`         | Programa principal que utiliza o analisador léxico. |
| `compile.sh`     | Script para compilação no Linux. |
| `compile.bat`    | Script para compilação no Windows. |
| `teste1.cshort`  | Arquivo de teste com código-fonte em Cshort. |
| `tokens.txt`     | Arquivo gerado com os tokens reconhecidos (output). |
| `Especificação Cshort-v1.0.pdf` | Documento com as regras léxicas e sintáticas da linguagem. |

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
