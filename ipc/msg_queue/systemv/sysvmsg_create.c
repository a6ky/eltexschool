#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int msqid, flags;
    unsigned int perms;
    key_t key;

    flags = IPC_CREAT;
    perms = S_IRUSR | S_IWUSR;

    key = ftok("./sysvmsg_create", 1);
    if (key == (key_t) -1)
        perror("ftok");

    msqid = msgget(key, flags | perms);
    if (msqid == -1)
        perror("msgget");

    printf("key:    %d\n", key);
    printf("msqid:  %d\n", msqid);

    exit(EXIT_SUCCESS);
} 