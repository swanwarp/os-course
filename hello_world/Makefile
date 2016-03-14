CC=gcc

all: hello_world

hello_world:
	$(CC) hello_world.c -o hello_world.o -c
	$(CC) -s hello_world.o -o hello_world

clean:
	rm -f *.o hello_world