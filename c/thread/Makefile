CC=gcc
SRCS=$(wildcard *.c)
BIN=test
CFLAGS=-g
LDFLAGS=-lpthread
OBJS=$(patsubst %c,%o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo $(SRCS)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
