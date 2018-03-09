BIN=pp
SRCS:=$(wildcard *.c)
CFLAGS= -g -Wall 
OBJS=$(patsubst %.c,%.o,$(SRCS))
LDFLAGS=-lpthread

.PHONY:all clean
all:$(BIN)

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

%.O:%.C
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET) $(BIN)
