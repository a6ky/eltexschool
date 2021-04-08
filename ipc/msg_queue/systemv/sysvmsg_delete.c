#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int j;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        printf("%s [msqid...]\n", argv[0]);

    for (j = 1; j < argc; j++)
        if (msgctl(atoi(argv[j]), IPC_RMID, NULL) == -1)
            perror("msgctl");

    exit(EXIT_SUCCESS);
}