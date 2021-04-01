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
    int msqid, msg_len;
    struct mbuf msg;
    key_t key;

    printf("Input message: ");
    fgets(msg.mtext, MSGLEN, stdin);

    msg_len = strlen(msg.mtext) + 1;

    key = ftok("./sysvmsg_create", 1);
    if (key == (key_t) -1)
        perror("ftok");

    msqid = msgget(key, 0);
    if (msqid == -1)
        perror("msgget");

    if (msgsnd(msqid, &msg, msg_len, 0) == -1)
        perror("msgsnd");
        
    exit(EXIT_SUCCESS);
}