.PHONY: asm, assemble

CC := gcc
BIN ?= ./assembler

ASM_SRC := $(wildcard asm/*.c)
ASM_OBJ := $(BIN)/asm.o


clean: ./bin
	rm -r ./bin
	rm -r $(BIN)

asm:
	@if [[! -d $(BIN)]]; then \
		mkdir -p $(BIN) \
	fi
	$(CC) $(ASM_SRC) -o $(ASM_OBJ)

assemble: $(ASM_OBJ)
	./$(ASM_OBJ) $(TARGET)