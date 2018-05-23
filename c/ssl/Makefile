CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lssl
TARGET = server client

.PHONY:all clean
all:$(TARGET)

server:ssl-server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
client:ssl-client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(TARGET)
