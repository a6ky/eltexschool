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
#define CLIENT_ADDR "127.0.0.1"
#define BUF_SIZE    1024
#define DGRAM_SIZE  4096

typedef struct udp_header {
    uint16_t src_port;          // порт источника
    uint16_t dst_port;          // порт назначения
    uint16_t len;               // длина заголовка UDP + данные
    uint16_t checksum;          // контрольная сумма UDP
} udp_header_t;

typedef struct ip_header {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t ihl:4;             // длинна заголовка IP-пакета в 32-битных словах
    uint16_t version:4;         // версия протокола IP
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t version:4;
    uint16_t ihl:4;
#else
#error "Please fix <bits/endian.h>"
#endif 
    u_char ds;                  // тип сервис
    uint16_t len;               // общая длина заголовка
    uint16_t id;                // идентификатор пакета
    uint16_t offset;            // смещение фрагмента
    u_char ttl;                 // время жизни
    u_char protocol;            // тип протокола
    uint16_t checksum;          // контрольная сумма
    uint32_t src_ip;            // IP-адрес отправителя
    uint32_t dst_ip;            // IP-адрес получателя
} ip_header_t;

#endif