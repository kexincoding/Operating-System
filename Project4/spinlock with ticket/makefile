export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
all:libticket.so libcounter.so liblist.so libhash.so

libticket.so: ticket.c
	gcc -c -fpic ticket.c -L.
	gcc -shared -o libticket.so ticket.o
libcounter.so: counter.c libticket.so
	gcc -L. -lticket -c -fpic counter.c 
	gcc -shared -o libcounter.so counter.o libticket.so
liblist.so: list.c libticket.so
	gcc -c -fpic list.c -lticket -L.
	gcc -shared -o liblist.so list.o libticket.so
libhash.so: hash.c liblist.so
	gcc -c -fpic hash.c -llist -L.
	gcc -shared -o libhash.so hash.o liblist.so
	rm -f *.o

clean:
	rm -f *.so
