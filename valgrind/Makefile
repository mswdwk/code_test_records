CC=gcc
SRCS=$(wildcard *.c)
SERVER=examp
#BIN=$(SERVER) $(CLIENT)
BIN=$(SERVER)
CFLAGS=-g -O0
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
	valgrind -s -v --log-file=valg1.log --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes  ./$(SERVER) 
