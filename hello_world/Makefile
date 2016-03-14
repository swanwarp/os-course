all: release	
hell.o: hello_world.c
	gcc -c hello_world.c -o hell.o
hello: hell.o
	gcc -s hell.o -o hello_world

release: hello
	rm -f hell.o
clean:
	rm -f hell.o hello_world
build:clean all