CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
LIBS = -lm

all: clean func

func: main.o functions.o
	$(CC) -o func main.o functions.o $(LIBS)

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	del -f *.o func
	del func.exe