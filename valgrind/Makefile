CC=gcc
SRCS=$(wildcard *.c)
SERVER=examp
#BIN=$(SERVER) $(CLIENT)
BIN=$(SERVER)
#CFLAGS=-g
LDFLAGS=-lpthread
#OBJS=$(patsubst %c,%o, $(SRCS))
SER_OBJS=examp.o
.PHONY:all clean
all:$(BIN)
	@echo "source files are:"$(SRCS)
	@echo "compile ok"
$(SERVER):$(SER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o $(BIN)
run:
	valgrind --leak-check=yes ./$(SERVER) 
