BIN=pp
SRCS:=$(wildcard *.c)
CFLAGS= -g -Wall 
CPPFLAGS= -g -Wall 
OBJS=$(patsubst %.c,%.o,$(SRCS))
LDFLAGS=-lpthread
RING_TEST=ring
SORT_TEST=sort
CPP=g++

TARGET=$(RING_TEST)  $(SORT_TEST)
.PHONY:all clean test

all:$(BIN) 

test:$(RING_TEST) $(SORT_TEST)

$(BIN): queue.o list.o pcap_parse.o  ring_buffer.o dump_print.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

%.O:%.C
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp	
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(RING_TEST):ring_buffer_test.o ring_buffer.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SORT_TEST):sort_test.o
	$(CPP) $(CPPFLAGS) $^ -o $@
clean:
	rm -rf *.o $(TARGET) $(BIN) *.log *.flv  *.txt
