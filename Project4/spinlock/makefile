export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

all: libspinlock.so libcounter.so libhash.so

libspinlock.so: spinlock.o
	gcc -shared -o libspinlock.so spinlock.o
libcounter.so: spinlock.o counter.o
	gcc -shared -o libcounter.so counter.o libspinlock.so
liblist.so: spinlock.o list.o
	gcc -shared -o liblist.so list.o libspinlock.so
libhash.so: spinlock.o hash.o liblist.so
	gcc -shared -o libhash.so hash.o libspinlock.so liblist.so
	rm -f *.o


spinlock.o: spinlock.c
	gcc -c -fpic spinlock.c -L.
counter.o: counter.c
	gcc -L. -lspinlock -c -fpic counter.c 
list.o: list.c list.h
	gcc -L. -lspinlock -c -fpic list.c 
hash.o: hash.c hash.h
	gcc -L. -lspinlock -llist -c -fpic hash.c  


clean:
	rm -f *.so 
