CC = gcc
CFLAGS = -g -Wall

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

EXEC = udp_server raw_ip_udp_client

all: $(EXEC)
	@rm -rf *.o

udp_server: $(OBJ)
	$(CC) $(CFLAGS) udp_server.o -o $@

raw_ip_udp_client: $(OBJ)
	$(CC) $(CFLAGS) raw_ip_udp_client.o -o $@

$(OBJ):	$(SRC)
	$(CC) $(SRC) -c

clean:
	@rm -rf *.o $(EXEC)