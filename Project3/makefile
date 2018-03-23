libmem.so:mem.c mem.h
	gcc -c -fpic mem.c -Wall -Werror
	gcc -shared -o libmem.so mem.o
clean:
	rm -f *.o

