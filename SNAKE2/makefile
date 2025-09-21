CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lncurses -DNCURSES_STATIC
LIBS = -lncurses
TARGET = snake
SOURCES = snake.c scheduler.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean