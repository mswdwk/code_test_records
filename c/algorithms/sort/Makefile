CC=gcc
CXX=g++
CFLAGS=-g -Wall
LDFLAGS=

TARGET1_SRC= quick_sort.c
TARGET1_OBJ=$(TARGET1_SRC:.c=.o)
TARGET1:= $(word 1, $(TARGET1_OBJ:.o=))
TARGET2_SRC= heap_sort.cpp
TARGET2_OBJ=$(TARGET2_SRC:.cpp=.o)
TARGET2:= $(word 1, $(TARGET2_OBJ:.o=))
#TARGET1= quick_sort

TARGET = $(TARGET1)  $(TARGET2) 

.PHONY:all clean
all:$(TARGET)

$(TARGET1):$(TARGET1_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2):$(TARGET2_OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
    
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)
sed_cn_char:
	sed -i 's/\o302|\0240//' *.cpp *.c
