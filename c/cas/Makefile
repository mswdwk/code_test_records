CC=gcc
SRCS=$(wildcard *.c)
SERVER=server
CLIENT=client
BIN=lock_free_queue lock_queue
CFLAGS=-g
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"

lock_free_queue:queue.o lock_free_queue.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

lock_queue:queue.o thread_queue.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

run:
	echo run	
clean:
	rm -rf *.o $(BIN)
tcp:
	tcpdump -tttt -nn -S -r tcp-ServerShutdownRd-ClientStillSendData.pcap 
	tcpdump -tttt -nn -S -r tcp-ServerShutdownWr-ClientStillSendData.pcap 
