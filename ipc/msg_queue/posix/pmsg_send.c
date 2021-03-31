#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    mqd_t mqd;
    char message[BUF_SIZE];
    unsigned int prio;

    mqd = mq_open("/mq", O_WRONLY);
    if (mqd == (mqd_t) -1)
        perror("mq_open");

    printf("Input message: ");
    fgets(message, BUF_SIZE, stdin);
    printf("Input priority: ");
    scanf("%u", &prio);

    if (mq_send(mqd, message, strlen(message), prio) == -1)
        perror("mq_send");

    exit(EXIT_SUCCESS);
} 