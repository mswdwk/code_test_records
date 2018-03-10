BIN=pp
SRCS:=$(wildcard *.c)
CFLAGS= -g -Wall 
OBJS=$(patsubst %.c,%.o,$(SRCS))
LDFLAGS=-lpthread
RING_TEST=ring
TARGET=$(RING_TEST) 

.PHONY:all clean
all:$(BIN) $(RING_TEST)

$(BIN):list.o pcap_parse.o  ring_buffer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

%.O:%.C
	$(CC) $(CFLAGS) -c $< -o $@

$(RING_TEST):ring_buffer_test.o ring_buffer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
clean:
	rm -rf *.o $(TARGET) $(BIN)
