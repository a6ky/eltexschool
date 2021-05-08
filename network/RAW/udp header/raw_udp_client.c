#include "udp_header.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr;
    udp_header_t        udp_header;
    uint16_t            dgram_len;
    int                 rawsock_fd;
    char                dgram[DGRAM_SIZE];
    char                data[BUF_SIZE];
    
    rawsock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (rawsock_fd == -1)
        handle_error("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);

    memset(data, 0, BUF_SIZE);
    strcpy(data, "my dgram");

    // общая длинна пакета
    dgram_len = sizeof(udp_header_t) + strlen(data);

    // формируем UDP заголовок
    udp_header.src_port = htons(CLIENT_PORT);
    udp_header.dst_port = htons(SERVER_PORT);
    udp_header.len      = htons(dgram_len);
    udp_header.checksum = 0;

    memcpy((void *) dgram, (void *) &udp_header, sizeof(udp_header_t));
    memcpy((void *) dgram + sizeof(udp_header_t), (void *) data, strlen(data));

    if (sendto(rawsock_fd, dgram, dgram_len, 0, (struct sockaddr *) &svaddr, 
                sizeof(struct sockaddr_in)) == -1)
            handle_error("sendto");

    if (close(rawsock_fd) == -1)
        handle_error("close");

    return 0;
}