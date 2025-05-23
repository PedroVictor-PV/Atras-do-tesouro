# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Arquivos
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/jogo.c $(SRC_DIR)/cliente_handler.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = $(BUILD_DIR)/main.exe

# Flags
CFLAGS = -I$(INC_DIR)
LIBS = -lws2_32

# Compilador
CC = gcc

# Regras
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	del /Q $(BUILD_DIR)\*.exe 2>nul || echo Nada para limpar

run: all
	$(TARGET)
