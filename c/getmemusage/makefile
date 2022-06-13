.PHONY:all clean
BIN=getmemusage
SRC=get_process_memusage.cc
all:
	g++ --std=c++11 $(SRC) -o $(BIN) 

clean:
	rm -rf *.o $(BIN)
fmt:
	indent -kr -i4 -ts4 -l88 -ss -bs -sob $(SRC)
