#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    mqd_t mqd;
    unsigned int prio;
    void *buffer;
    struct mq_attr attr;
    ssize_t num_read;

    mqd = mq_open("/mq", O_RDONLY);
    if (mqd == (mqd_t) -1)
        perror("mq_open");

    if (mq_getattr(mqd, &attr) == -1)
        perror("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        perror("malloc");

    num_read = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
    if (num_read == -1)
        perror("mq_send");

    printf("Read %ld bytes; priority = %u\n", (long) num_read, prio);

    if (write(STDOUT_FILENO, buffer, num_read) == -1)
        perror("write");

    write(STDOUT_FILENO, "\n", 1);
    exit(EXIT_SUCCESS);
} 