CC=gcc
SRCS=$(wildcard *.c)
BIN=thread_affinity setThreadName
CFLAGS=-g
LDFLAGS=-lpthread
OBJS=$(patsubst %c,%o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo $(SRCS)
%.o:%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LDFLAGS)
#$(BIN):$(OBJS)
#	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
