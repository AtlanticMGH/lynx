TARGET = lynx

#compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99

#source files
SRCS = main.c shell_builtins.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
