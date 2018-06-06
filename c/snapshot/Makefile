CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
TARGET = memsnap

.PHONY:all clean
all:$(TARGET)

memsnap:memory_snap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)
