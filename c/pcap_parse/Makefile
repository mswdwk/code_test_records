BIN=pp
CC=gcc
SRCS:=$(wildcard *.c)
CFLAGS= -g -Wall 
CPPFLAGS= -g -Wall 
OBJS=$(patsubst %.c,%.o,$(SRCS))
LDFLAGS=-lpthread
RING_TEST=ring
SORT_TEST=sort
CPP=g++
LIBS_A=libalgorithm.a

TARGET=$(RING_TEST)  $(SORT_TEST) $(LIBS_A)
.PHONY:all clean test

all:$(BIN) $(LIBS_A)

test:$(RING_TEST) $(SORT_TEST)

$(BIN): queue.o list.o pcap_parse.o  ring_buffer.o dump_print.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.O:%.C
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp	
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(RING_TEST):ring_buffer_test.o ring_buffer.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SORT_TEST):sort_test.o
	$(CPP) $(CPPFLAGS) $^ -o $@
<<<<<<< HEAD
$(LIBS_A):ring_buffer.o queue.o list.o dump_print.o
	ar rcs $@ $^
clean:
	rm -rf *.o $(TARGET) $(BIN) *.log *.flv $(LIBS_A)
