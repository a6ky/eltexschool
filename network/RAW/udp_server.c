#include "udp_header.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in  server_addr, client_addr;
    ssize_t             num_bytes;
    socklen_t           addr_len;
    int                 sock_fd, index;
    char                buf[BUF_SIZE];
    char                ipv4[INET_ADDRSTRLEN];

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd == -1)
        handle_error("socket");

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(server_addr.sin_addr));
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    for (;;) {
        addr_len = sizeof(struct sockaddr_in);
        memset(buf, 0, BUF_SIZE);
        num_bytes = recvfrom(sock_fd, buf, BUF_SIZE, 0, 
                            (struct sockaddr *) &client_addr, &addr_len);

        if (num_bytes == 0)
            break;

        if (num_bytes == -1)
            handle_error("recvfrom");

        printf("Server received %ld bytes from %s\n", (long) num_bytes, 
                inet_ntop(AF_INET, &client_addr.sin_addr, ipv4, INET_ADDRSTRLEN));
        printf("buf: %s\n", buf);

        for (index = 0; index < num_bytes; index++)
            buf[index] = toupper((unsigned char) buf[index]);

        if ((sendto(sock_fd, buf, num_bytes, 0, 
                  (struct sockaddr *) &client_addr, addr_len)) == -1)
            handle_error("sendto");
    }

    if (close(sock_fd) == -1)
        handle_error("close");

    return 0;
}