# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source directory
SRC_DIR = src

# Header directory
INC_DIR = include

# Main file
MAIN = main.c

# Source files: all .c files in the source directory plus the main file
SRCS = $(wildcard $(SRC_DIR)/*.c) $(MAIN)

# Object files: replace .c with .o
OBJS = $(patsubst %.c,%.o,$(SRCS))

# Executable name
TARGET = SO_Pthreads

# Phony targets
.PHONY: all compile run clean

# Default target (compile and run)
all: compile run

# Explicit compile target (produces the target program)
compile: $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Compile source files in the source directory into object files:
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm