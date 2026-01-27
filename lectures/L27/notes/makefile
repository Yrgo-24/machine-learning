# Application target.
TARGET := flatten_demo

# C++ compiler.
CXX_COMPILER := g++

# Source files.
SOURCE_FILES := flatten_demo.cpp \

# Compiler flags.
CXX_FLAGS := -std=c++17 -Wall -Werror

# Build and run the application as default.
default: build run

# Build the application.
build:
	@$(CXX_COMPILER) $(SOURCE_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)