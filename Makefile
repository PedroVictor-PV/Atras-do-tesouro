# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Arquivos fonte
SOURCES = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/jogo.c \
	$(SRC_DIR)/cliente_handler.c \
	$(SRC_DIR)/threads.c

# Arquivo de saída
TARGET = $(BUILD_DIR)/main.exe

# Flags de compilação
CFLAGS = -I$(INC_DIR)
LIBS = -lws2_32

# Compilador
CC = gcc

# Regra padrão
all: $(TARGET)

# Compilação
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

# Limpeza
clean:
	del /Q $(BUILD_DIR)\*.exe 2>nul || echo Nada para limpar

# Execução
run: all
	$(TARGET)
