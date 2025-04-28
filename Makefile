# Compiler
CC = gcc

# Compiler flags (for debugging and optimization)
CFLAGS = -Wall -g -I./include

# Source and Object files
SRC = src/main.c
OBJ = $(SRC:.c=.o)

# Output binary name
OUT = riscv_emulator

# Default target to build the project
all: $(OUT)

# Link object files to create the executable
$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

# Compile the source file to object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(OUT)

# Run the emulator after building it
run: $(OUT)
	./$(OUT)
