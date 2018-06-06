CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
TARGET1= bitmap_test
TARGET = $(TARGET1)

.PHONY:all clean
all:$(TARGET)

$(TARGET1):bitmap_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)
