# Define the compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Iengine/includes -MMD -MP -I/usr/include/vulkan -I/usr/include/xcb
LDFLAGS :=
LIBS := -lm -lvulkan -lxcb
CHECK_LIBS := -lcheck -lsubunit -lm

# Define directories
ENGINE_DIR := engine
BIN_DIR := bin
TEST_DIR := tests

# Define the main executable and test executable names
EXEC := YumeEngine
TEST_EXEC := $(BIN_DIR)/test_suite

# Find all C source files in the engine directory and its subdirectories
SRCS := $(shell find $(ENGINE_DIR) -type f -name '*.c')
# Generate object file names by preserving the directory structure
OBJS := $(SRCS:$(ENGINE_DIR)/%.c=$(BIN_DIR)/%.o)

# Find all test source files
TEST_SRCS := $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BIN_DIR)/%.o)

# Dependency files
DEPS := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all clean test

# Default target
all: $(EXEC)

# Link the main executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ $(LIBS)

# Compile source files into object files
$(BIN_DIR)/%.o: $(ENGINE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Test target
test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Link the test executable
# Make sure to include all necessary libraries
$(TEST_EXEC): $(TEST_OBJS) $(filter-out $(BIN_DIR)/src/main.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(CHECK_LIBS) -lm

# Compile test source files into object files
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR)/*

# Include the dependency files
-include $(DEPS)
