#include "tcp_header.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr;
    int                 sfd, cfd;
    char                buf[BUF_SIZE];

    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd == -1)
        handle_error("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);

    cfd = connect(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in));
    if (cfd == -1)
        handle_error("connect");

    for (;;) {  
        printf("Send message to server: ");
        fgets(buf, BUF_SIZE, stdin);

        if (send(sfd, buf, BUF_SIZE, 0) == -1)
            handle_error("send");

        if (recv(sfd, buf, BUF_SIZE, 0) == -1)
            handle_error("recv");

        printf("Recieve message from server: %s\n", buf);
    }

    if (close(sfd) == -1)
        handle_error("close");

    return 0;
}