SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
TARGET = shell

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS) *.dSYM

re: clean all

.PHONY: all clean re
