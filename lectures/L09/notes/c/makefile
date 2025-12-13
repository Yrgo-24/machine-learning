# Application target.
TARGET := app

# Build and run the application as default.
default: build run

# Build the application.
build:
	@g++ main.c -o $(TARGET) -Wall -Werror

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)