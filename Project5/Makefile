CC = gcc
CFLAGS = -Wall

defrag: defragmenter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm 

.PHONY: clean
clean:
	-rm *.o defrag
