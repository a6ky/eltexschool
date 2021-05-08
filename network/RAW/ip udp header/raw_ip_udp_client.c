#include "udp_header.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr;
    ip_header_t         ip_header;
    udp_header_t        udp_header;
    uint16_t            dgram_len;
    uint16_t            udp_len;
    int                 rawsock_fd;
    int                 val;          
    char                dgram[DGRAM_SIZE];
    char                data[BUF_SIZE];
    
    rawsock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (rawsock_fd == -1)
        handle_error("socket");

    if (setsockopt(rawsock_fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0)
        handle_error("setsockopt");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);

    memset(data, 0, BUF_SIZE);
    strcpy(data, "my dgram");

    // общая длинна пакета
    dgram_len = sizeof(ip_header_t) + sizeof(udp_header_t) + strlen(data);
    udp_len = sizeof(udp_header_t) + strlen(data);

    // формируем IP заголовок
    ip_header.ihl       = 5;
    ip_header.version   = 4;
    ip_header.ds        = 0;
    ip_header.len       = htons(dgram_len);
    ip_header.offset    = 0;
    ip_header.ttl       = 255;
    ip_header.protocol  = IPPROTO_UDP;
    ip_header.checksum  = 0;
    ip_header.src_ip    = inet_pton(AF_INET, CLIENT_ADDR, &(ip_header.src_ip));
    ip_header.src_ip    = inet_pton(AF_INET, SERVER_ADDR, &(ip_header.dst_ip));

    // формируем UDP заголовок
    udp_header.src_port = htons(CLIENT_PORT);
    udp_header.dst_port = htons(SERVER_PORT);
    udp_header.len      = htons(udp_len);
    udp_header.checksum = 0;

    memcpy(dgram, &ip_header, sizeof(ip_header_t));
    memcpy(dgram + sizeof(ip_header_t), &udp_header, sizeof(udp_header_t));
    memcpy(dgram + sizeof(ip_header_t) + sizeof(udp_header_t), data, strlen(data));

    if (sendto(rawsock_fd, dgram, dgram_len, 0, (struct sockaddr *) &svaddr, 
                sizeof(struct sockaddr_in)) == -1)
            handle_error("sendto");

    if (close(rawsock_fd) == -1)
        handle_error("close");

    return 0;
}