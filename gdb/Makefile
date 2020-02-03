CC=gcc
SRCS=$(wildcard *.c)
SERVER=notbedebuged
#BIN=$(SERVER) $(CLIENT)
BIN=$(SERVER)
#CFLAGS=-g
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
SER_OBJS=notbedebuged.o
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"
$(SERVER):$(SER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
tcp:
	tcpdump -tttt -nn -S -r tcp-ServerShutdownRd-ClientStillSendData.pcap 
	tcpdump -tttt -nn -S -r tcp-ServerShutdownWr-ClientStillSendData.pcap 
