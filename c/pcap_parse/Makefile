BIN_TCP_RTP=tcprtp
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

TARGET=$(RING_TEST)  $(SORT_TEST) $(LIBS_A) $(BIN_TCP_RTP)
.PHONY:all clean test

all:$(BIN) $(LIBS_A) $(BIN_TCP_RTP)

test:$(RING_TEST) $(SORT_TEST)

$(BIN): queue.o list.o pcap_parse.o  ring_buffer.o dump_print.o ip_flow.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp	
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(RING_TEST):ring_buffer_test.o ring_buffer.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SORT_TEST):sort_test.o
	$(CPP) $(CPPFLAGS) $^ -o $@
$(LIBS_A):ring_buffer.o queue.o list.o dump_print.o ip_flow.o
	ar rcs $@ $^
$(BIN_TCP_RTP): pcap_parse_tcp_rtp.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS_A) 
clean:
	rm -rf *.o $(TARGET) $(BIN) *.log *.flv $(LIBS_A)
