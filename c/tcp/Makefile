CC=gcc
SRCS=$(wildcard *.c)
SERVER=server
CLIENT=client
BIN=$(SERVER) $(CLIENT)
CFLAGS=-g
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
CLI_OBJS=client.o
SER_OBJS=server.o
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"
$(CLIENT):$(CLI_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(SERVER):$(SER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
tcp:
	tcpdump -tttt -nn -S -r tcp-ServerShutdownRd-ClientStillSendData.pcap 
	tcpdump -tttt -nn -S -r tcp-ServerShutdownWr-ClientStillSendData.pcap 
