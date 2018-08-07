CC=g++
CFLAGS=-c -O2 

all: cmd_pcap

cmd_pcap: hw5.o
	$(CC) hw5.o -o cmd_pcap -lpcap

hw5.o: hw5.cpp
	$(CC) $(CFLAGS) hw5.cpp

clean:
	/bin/rm -f cmd_pcap *.o

run:
	sudo ./cmd_pcap

tarball:
	# put your tar command here
	tar -cvzf spiess.tar.gz *

