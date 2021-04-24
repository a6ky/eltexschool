CC = gcc
CFLAGS = -g -Wall

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

EXEC = udp_server udp_client

all: $(EXEC)
	@rm -rf *.o

udp_server: $(OBJ)
	$(CC) $(CFLAGS) udp_server.o -o $@

udp_client: $(OBJ)
	$(CC) $(CFLAGS) udp_client.o -o $@

$(OBJ):	$(SRC)
	$(CC) $(SRC) -c

clean:
	@rm -rf *.o $(EXEC)