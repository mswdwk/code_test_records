CC=gcc
SRCS=$(wildcard *.c)
CLIENT=getru 
BIN=$(CLIENT) getru2  getrthread
#CFLAGS=-g
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
CLI_OBJS=get_resource_usage.o
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS)

$(CLIENT):$(CLI_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

getru2:get_resource_usage_2.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
getrthread:get_resource_usage_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
tcp:
	tcpdump -tttt -nn -S -r tcp-ServerShutdownWr-ClientStillSendData.pcap 

te:
	@echo $$$$
te2:
	@echo $$
te3:
	@echo $$?
