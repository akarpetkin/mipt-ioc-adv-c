GCC=gcc
CFLAGS=-Wall -Wextra -std=c99 -lncurses -DNCURSES_STATIC -g
LIBS = -lncurses

all: clean snake

snake.o: snake.c
	$(GCC) $(CFLAGS) -c -o snake.o snake.c

scheduler.o: scheduler.c
	$(GCC) $(CFLAGS) -c -o scheduler.o scheduler.c

snake: snake.o scheduler.o
	$(GCC) $(CFLAGS) -o snake snake.o scheduler.o $(LIBS)

clean:
	del *.o
	del *.exe