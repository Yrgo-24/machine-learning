# Application target.
TARGET=main

# Source files.
SOURCE_FILES=source/atmega328p_led.cpp\
             source/main.cpp\
			 source/stm32f103_led.cpp\

# Build and run the application as default.
default: build run

# Build the application.
build:
	@g++ $(SOURCE_FILES) -Iinclude -o $(TARGET) -Wall -Werror

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)
