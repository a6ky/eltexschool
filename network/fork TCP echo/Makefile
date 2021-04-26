CC = gcc
CFLAGS = -g -Wall

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

EXEC = tcp_server tcp_client

all: $(EXEC)
	@rm -rf *.o

tcp_server: $(OBJ)
	$(CC) $(CFLAGS) tcp_server.o -o $@

tcp_client: $(OBJ)
	$(CC) $(CFLAGS) tcp_client.o -o $@

$(OBJ):	$(SRC)
	$(CC) $(SRC) -c

clean:
	@rm -rf *.o $(EXEC)