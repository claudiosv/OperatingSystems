CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw3

hw3: dpsim.o hw3.o
	$(CC) dpsim.o hw3.o -o hw3 -lpthread

dpsim.o: dpsim.c
	$(CC) $(CFLAGS) dpsim.c

hw3.o: hw3.c
	$(CC) $(CFLAGS) hw3.c

clean:
	/bin/rm -f hw3 *.o *.gz

run:
	./hw3

tarball:
	# put your tar command here
	tar -cvzf spiess.tar.gz *

