# Makefile para o compilador Cshort (léxico + sintático)

# Diretórios
SRC_DIR = src
LEX_DIR = $(SRC_DIR)/lex
SINT_DIR = $(SRC_DIR)/sint
BIN_DIR = bin
OUT_DIR = out
TEST_DIR = test

# Arquivos-fonte
LEX_SRC = $(LEX_DIR)/anaLex.c
SINT_SRC = $(SINT_DIR)/anaSint.c
MAIN_SRC = $(SINT_DIR)/main.c

# Nome do executável
EXEC = $(BIN_DIR)/cshort.out

# Flags de compilação
CFLAGS = -Wall -Wextra

all: $(EXEC)

$(EXEC): $(LEX_SRC) $(SINT_SRC) $(MAIN_SRC)
	@mkdir -p $(BIN_DIR) $(OUT_DIR)
	gcc $(LEX_SRC) $(SINT_SRC) $(MAIN_SRC) -o $(EXEC) $(CFLAGS)
	@echo "✅ Compilado com sucesso: $(EXEC)"

run: all
	@echo "▶️ Executando $(EXEC) com arquivo de teste..."
	$(EXEC) $(TEST_DIR)/teste_erro1.cshort

lex:
	@mkdir -p $(BIN_DIR)
	gcc $(LEX_SRC) $(SRC_DIR)/main.c -o $(BIN_DIR)/lex.out $(CFLAGS)
	@echo "✅ Compilado apenas o analisador léxico: $(BIN_DIR)/lex.out"

clean:
	rm -rf $(BIN_DIR)/* $(OUT_DIR)/*
	@echo "🧹 Diretórios bin/ e out/ limpos."

.PHONY: all run lex clean
