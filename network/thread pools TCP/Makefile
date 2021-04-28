CC = gcc
CFLAGS = -g -Wall

BINS = tcp_server
OBJ = tcp_server.o myqueue.o

all: $(BINS) tcp_client
	@rm -rf *.o

tcp_server: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -pthread

%.o: %.c
	$(CC) $(SRC) -c -o $@ $^

clean:
	@rm -rf *.o $(BINS)