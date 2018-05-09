CC=g++
SRCS=$(wildcard *.cpp)
BIN=test
CFLAGS=-g -Wall
LDFLAGS=-lboost_thread -lboost_system
OBJS=$(patsubst %.cpp,%.o, $(SRCS))
.PHONY:all clean
all:$(BIN)
	@echo $(SRCS)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(BIN)
