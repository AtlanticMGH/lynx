TARGET = myshell

#compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

#source files
SRCS = main.c shell_builtins.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)