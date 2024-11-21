BIN_DIR := bin
INCLUDE_DIR := include
LIB_DIR := lib
OBJ_DIR := obj
SRC_DIR := src

_DEPS := builtins.h process.h
DEPS := $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJ := main.o builtins.o process.o
OBJ := $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

TARGET := $(BIN_DIR)/bmblsh

CC := cc
CFLAGS := -Wall -Wextra -pedantic -I$(INCLUDE_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAG) -o $@ $^ $(LIBS)

.PHONY: clean run

clean:
	rm -f $(OBJ_DIR)/*.o *~ core $(INCLUDE_DIR)/*~

run: $(TARGET)
	./$(TARGET)
