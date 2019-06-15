CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lpthread

TARGET1_SRC= main.c thread_pool.c
TARGET1_OBJ=$(TARGET1_SRC:.c=.o)
TARGET1:= $(word 1, $(TARGET1_OBJ:.o=))
#TARGET1= quick_sort

TARGET = $(TARGET1)

.PHONY:all clean
all:$(TARGET)

$(TARGET1):$(TARGET1_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)
