.PHONY:all clean
BIN=detect
all:
	g++ --std=c++11 detect.cc -o $(BIN) 

clean:
	rm -rf *.o $(BIN)
fmt:
	indent -kr -i4 -ts4 -l88 -ss -bs -sob detect.cc
