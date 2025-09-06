# Application target.
TARGET := main

# Source files.
SOURCE_FILES := main.cpp\
                components/source/router/stub.cpp\
                components/source/sw/stub.cpp\

# Include directories.
INCLUDE_DIRS := components/include/

# Additional compiler flags.
COMPILER_FLAGS := -Wall -Werror -std=c++17

# Build and run the application as default.
default: build run

# Build the application.
build:
	@g++ $(SOURCE_FILES) -o $(TARGET) -I$(INCLUDE_DIRS) $(COMPILER_FLAGS)

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)
