#ifndef UDP_HEADER
#define UDP_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define SERVER_PORT 7766
#define CLIENT_PORT 6677
#define SERVER_ADDR "127.0.0.1"
#define BUF_SIZE    1024
#define DGRAM_SIZE  4096

typedef struct udp_header {
    uint16_t src_port;          // порт источника
    uint16_t dst_port;          // порт назначения
    uint16_t len;               // длина заголовка UDP + данные
    uint16_t checksum;          // контрольная сумма UDP
} udp_header_t;

#endif