#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

#define MSGLEN 1024

struct mbuf {
    long mtype;                     /* Message type */
    char mtext[MSGLEN];             /* Message body */
};

int main(int argc, char *argv[])
{
    int msqid;
    ssize_t msg_len;
    struct mbuf msg;
    key_t key;

    key = ftok("./sysvmsg_create", 1);
    if (key == (key_t) -1)
        perror("ftok");

    msqid = msgget(key, 0);
    if (msqid == -1)
        perror("msgget");

    msg_len = msgrcv(msqid, &msg, MSGLEN, 0, 0);
    if (msg_len == -1)
        perror("msgrcv");

    printf("Recived: type = %ld; length = %ld", msg.mtype, (long) msg_len);
    if (msg_len > 0)
        printf("; body = %s", msg.mtext);
    printf("\n");

    exit(EXIT_SUCCESS);
}