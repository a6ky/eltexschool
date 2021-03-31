#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int flags;
    mode_t perms;
    mqd_t mqd;
    struct mq_attr attr;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 2048;
    flags = O_RDWR | O_CREAT;
    perms = S_IRUSR | S_IWUSR;

    mqd = mq_open("/mq", flags, perms, &attr);
    if (mqd == (mqd_t) -1)
        perror("mq_open");

    exit(EXIT_SUCCESS);
}