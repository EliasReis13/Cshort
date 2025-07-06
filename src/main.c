#include <stdio.h>
#include "sint/anaSint.h" 

FILE *fd;
TOKEN t;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.cshort>\n", argv[0]);
        return 1;
    }

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    printf("Diretorio de trabalho atual: %s\n", cwd);
    printf("Tentando abrir o arquivo (argv[1]): %s\n", argv[1]);

    fd = fopen(argv[1], "r");
    if (!fd) {
        printf("Erro ao abrir arquivo: %s\n", argv[1]);
        return 1;
    }

    // A mágica acontece aqui: inicia a análise sintática
    Prog(); // O analisador sintático agora controla a chamada ao léxico

    fclose(fd);
    
    // Opcional: verificar se ocorreram erros sintáticos
    if (houveErroSintatico) {
         printf("\nCompilacao encerrada com erros.\n");
         return 1;
    }

    printf("\nCompilacao concluida com sucesso.\n");
    return 0;
}