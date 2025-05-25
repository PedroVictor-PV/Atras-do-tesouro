CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRC_DIR = src
BIN_DIR = bin

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=%.o)

all: $(BIN_DIR)/servidor.exe $(BIN_DIR)/client.exe

$(BIN_DIR)/servidor.exe: $(SRCS)
    $(CC) $(CFLAGS) -o $@ $(filter %servidor.c %Main.c,$^) $(filter-out %client.c,$(filter-out %cliente_handler.c,$(filter-out %main.c,$(SRCS))))

$(BIN_DIR)/client.exe: $(SRCS)
    $(CC) $(CFLAGS) -o $@ $(filter %client.c %cliente_handler.c %main.c,$^) $(filter-out %servidor.c,$(filter-out %Main.c,$(SRCS))))

clean:
    rm -f $(BIN_DIR)/*.exe
    rm -f $(SRC_DIR)/**/*.o

.PHONY: all clean