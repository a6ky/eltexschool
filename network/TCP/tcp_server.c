#include "tcp_header.h"

/*
 * BACKLOG - максимальная длина, до которой может 
 *           расти очередь ожидающих соединений
 */
#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr;
    int                 sfd, cfd;
    ssize_t             num_read;
    char                buf[BUF_SIZE];

    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd == -1)
        handle_error("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    if (listen(sfd, BACKLOG) == -1)
        handle_error("listen");
        
    for (;;) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
            handle_error("accept");

        num_read = recv(cfd, buf, BUF_SIZE, 0);

        if (num_read == 0)
            break;

        if (num_read == -1)
            handle_error("recv");

        printf("Receive message from client: %s\n", buf);
        
        if (send(cfd, buf, BUF_SIZE, 0) == -1)
            handle_error("send");

        if (close(cfd) == -1)
            handle_error("close");
    }
    
    if (close(sfd) == -1)
        handle_error("close");

    return 0;
}