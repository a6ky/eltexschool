#ifndef TCP_HEADER
#define TCP_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define SERVER_PORT 6677
#define SERVER_ADDR "127.0.0.1"
#define BUF_SIZE 256

#endif