# Define compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Iengine/include
LDFLAGS :=

# Define paths
SRC_DIR := engine/src
INC_DIR := engine/include
BUILD_DIR := bin
TEST_DIR := test/src

# Define source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_LIBS := `pkg-config --libs check`

# Define your executable name
EXEC := YumeEngine

.PHONY: all clean test

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$@ $(LDFLAGS)

# Compile the source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)/*

# Add a new target for test
test: $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_main $(TEST_DIR)/test_main.c $(LDFLAGS) $(TEST_LIBS)
	./$(BUILD_DIR)/test_main
