# Compiler
CC = gcc

# Compiler flags (for debugging and optimization)
CFLAGS = -Wall -g -I./include

# Source and Object files for the emulator
SRC = src/main.c src/cpu.c src/instruction_set.c src/dram.c
OBJ = $(SRC:.c=.o)

# Test files
TEST_SRC = tests/instruction_set_test.c tests/dram_test.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Output binary names
OUT = riscv_emulator
TEST_OUT = instruction_set_test dram_test

# Default target to build the emulator
all: $(OUT)

# Build the emulator
$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

# Build test binaries
instruction_set_test: tests/instruction_set_test.c src/cpu.c src/instruction_set.c src/dram.c
	$(CC) $(CFLAGS) $^ -o $@

dram_test: tests/dram_test.c src/dram.c
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the emulator
run: $(OUT)
	./$(OUT)

# Run tests
test: instruction_set_test dram_test
	./instruction_set_test
	./dram_test

# Clean up object files and binaries
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(OUT) $(TEST_OUT)

.PHONY: all run test clean