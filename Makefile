.PHONY: clean asm assemble

CC := gcc
BIN ?= assembler
ASM_TARGET ?= a.asm

ASM_SRC := $(wildcard asm/*.c)
ASM_OBJ := $(BIN)/asm


clean_asm: $(BIN)
	rm -r $(BIN)

asm: $(ASM_SRC)
	mkdir -p $(BIN)
	$(CC) $(ASM_SRC) -o $(ASM_OBJ)

assemble: $(ASM_OBJ)
	./$(ASM_OBJ) $(ASM_TARGET)