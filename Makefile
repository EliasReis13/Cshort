CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/lexer/lexer.c
OUT = build/cshort

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build
