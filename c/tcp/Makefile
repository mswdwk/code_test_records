Q=
CC=gcc
SRCS=$(wildcard *.c)
SERVER=server
CLIENT=client
BIN=$(SERVER) $(CLIENT)
CFLAGS=-g -Wall
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
CLI_OBJS=client.o
SER_OBJS=server.o test_func.o
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"
$(CLIENT):$(CLI_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(SERVER):$(SER_OBJS)
	$(CC) $(CFLAGS)  -o $@ $^  #$(LDFLAGS) 
%.o:%.c
	$Q$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(BIN) 
tcp:
	tcpdump -tttt -nn -S -r tcp-ServerShutdownRd-ClientStillSendData.pcap 
	tcpdump -tttt -nn -S -r tcp-ServerShutdownWr-ClientStillSendData.pcap 
clean_pcap:
	rm -rf *.pcap
