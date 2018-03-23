export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH


all: libmutex.so libmutex_counter.so libmutex_list.so libmutex_hash.so
libmutex.so: 2pl.o
	gcc -shared -o libmutex.so 2pl.o
libmutex_counter.so: mutex_counter.o libmutex.so
	gcc -shared -o libmutex_counter.so mutex_counter.o libmutex.so
libmutex_list.so: mutex_list.o libmutex.so
	gcc -shared -o libmutex_list.so mutex_list.o libmutex.so
libmutex_hash.so: mutex_hash.o libmutex_list.so
	gcc -shared -o libmutex_hash.so mutex_hash.o libmutex_list.so
	rm -f *.o



2pl.o: 2pl.c
	gcc -c -fpic 2pl.c -L.
mutex_counter.o: mutex_counter.c
	gcc -L. -lmutex -c -fpic mutex_counter.c
mutex_list.o: mutex_list.c 
	gcc -c -fpic mutex_list.c -lmutex -L.
mutex_hash.o: mutex_hash.c
	gcc -c -fpic mutex_hash.c -lmutex_list -L.

clean:
	rm -f *.so
	

