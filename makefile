all: filosofer.o filosofer

clean:
	rm -f *.o
	rm -f filosofer

CC= gcc -std=c99 -g
CFLAGS=-lpthread
DEPS = filosofer.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filosofer: filosofer.o 
	$(CC) -o filosofer filosofer.o -lpthread

filosofer.o: filosofer.c filosofer.h
	$(CC) -c filosofer.c -lpthread



