#ifndef TCP_HEADER_H_
#define TCP_HEADER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define SERVER_PORT 6677
#define SERVER_ADDR "127.0.0.1"
#define BUF_SIZE 4096
#define THREAD_POOL_SIZE 5

#endif