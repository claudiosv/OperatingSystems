CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw4

hw4: binary_semaphore.o hw4.o
	$(CC) binary_semaphore.o hw4.o -o hw4 -lpthread 

binary_semaphore.o: binary_semaphore.c
	$(CC) $(CFLAGS) binary_semaphore.c

hw4.o: hw4.c
	$(CC) $(CFLAGS) hw4.c

clean:
	/bin/rm -f hw4 *.o *.gz

run:
	./hw4 8 4 10 

tarball:
	# put your tar command here
	tar -cvzf spiess.tar.gz *

