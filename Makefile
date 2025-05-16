# Nome do executável
TARGET = treasure_game

# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Arquivos fontes
SRCS = $(SRC_DIR)/Main.c $(SRC_DIR)/jogo.c

# Flags
CFLAGS = -I$(INCLUDE_DIR)

# Regra padrão
all: $(BUILD_DIR)/$(TARGET).exe

$(BUILD_DIR)/$(TARGET).exe: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

# Limpar arquivos compilados
clean:
	del /Q $(BUILD_DIR)\*.exe 2>nul || true

# Rodar o programa
run: all
	./$(BUILD_DIR)/$(TARGET).exe
