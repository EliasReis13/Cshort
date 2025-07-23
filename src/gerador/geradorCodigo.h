/**
 * @file geradorCodigo.h
 * @brief Definição da Interface Pública para o Módulo Gerador de Código
 *
 * @purpose
 * Este arquivo de cabeçalho define os protótipos das funções que o analisador
 * sintático utiliza para gerar o código intermediário para a máquina de pilha.
 * Ele abstrai os detalhes da implementação, como o armazenamento do código
 * em um buffer
 */

#ifndef GERADOR_CODIGO_H
#define GERADOR_CODIGO_H

// Define o tamanho máximo de uma linha de instrução gerada.
#define TAM_LINHA 100   


/**
 * @brief Adiciona uma instrução de máquina de pilha ao buffer de código.
 * @param instrucao A string completa da instrução (ex: "PUSH 5" ou "ADD").
 */
void gera(char *instrucao);

/**
 * @brief Gera e retorna um número de rótulo único.
 * @return Um inteiro que representa o novo rótulo (0, 1, 2, ...).
 */
int novo_rotulo();

/**
 * @brief Gera a instrução de um rótulo no formato "Lr:" e a adiciona ao buffer.
 * @param r O número do rótulo a ser gerado.
 */

void gera_rotulo(int r);

/**
 * @brief Escreve todo o conteúdo do buffer de código em um arquivo de texto.
 * Função chamada no final do processo de compilação.
 * @param nome_arquivo O caminho do arquivo onde o código será salvo.
 */
void salvar_codigo_em_arquivo(const char *nome_arquivo);

#endif // GERADOR_CODIGO_H