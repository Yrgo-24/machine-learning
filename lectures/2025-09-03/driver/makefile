# Build and run the application as default.
default: build run

# Source files.
SOURCE_FILES=source/atmega328p_led.cpp\
             source/main.cpp\
			 source/stm32_led.cpp\

# Build the application.
build:
	@g++ $(SOURCE_FILES) -Iinclude -o main -Wall -Werror

# Run the application.
run:
	@./main

# Clean the application.
clean:
	@rm -f main
