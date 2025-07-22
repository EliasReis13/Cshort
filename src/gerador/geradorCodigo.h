#ifndef GERADOR_CODIGO_H
#define GERADOR_CODIGO_H

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
 * @brief Escreve todo o conteúdo do buffer de código em um arquivo.
 * @param nome_arquivo O caminho do arquivo onde o código será salvo.
 */
void salvar_codigo_em_arquivo(const char *nome_arquivo);

#endif // GERADOR_CODIGO_H