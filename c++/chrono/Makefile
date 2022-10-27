CC=g++
SRCS=$(wildcard *.cc)
BIN=cost_time
CFLAGS=-g -std=c++11
LDFLAGS=-lpthread
OBJS=$(patsubst %cc,%o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo "source files are: "$(SRCS)
	@echo "compile ok"

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	
%.o:%.cc
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf *.o $(BIN) *.pcap
