#include "udp_header.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr, claddr;
    int                 sfd;
    size_t              msg_len;   
    ssize_t             num_bytes;           
    char                buf[BUF_SIZE];

    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sfd == -1)
        handle_error("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);
    
    for (;;) {  
        printf("Send message to server: ");
        memset (buf, 0, BUF_SIZE);
        fgets(buf, BUF_SIZE, stdin);

        msg_len = strlen(buf);
        buf[msg_len - 1] = '\0';

        if (sendto(sfd, buf, msg_len, 0, (struct sockaddr *) &svaddr, 
                  sizeof(struct sockaddr_in)) != msg_len)
            handle_error("sendto");

        num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
        if (num_bytes == -1)
            handle_error("recvfrom");

        printf("Recieve message from server: %s\n", buf);
    }

    if (close(sfd) == -1)
        handle_error("close");

    return 0;
}