CC=g++
SRCS=$(wildcard *.cc)
BIN=test
CFLAGS=-g -Wall -std=c++11
LDFLAGS=-lboost_unit_test_framework
OBJS=$(patsubst %.cc,%.o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo $(SRCS)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(BIN)
