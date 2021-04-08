#include "svmsg_header.h"

static int clientId;

static void removeQeue(void)
{
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        perror("msgctl");
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("%s pathname\n", argv[0]);

    /* Получаем идентификатор очереди сообщений сервера */

    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
        perror("msgget - server message queue");

    /* Создаем очередь для ответа */

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        perror("msgget - client message queue");

    if (atexit(removeQeue) != 0)
        perror("atexit");

    req.mtype = 1;                  /* Указываем любой тип */
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';

    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
        perror("msgsnd");

    /* Получаем первый ответ, который может быть уведомлением о сбое */

    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1)
        perror("msgrcv");

    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);
        if (msgctl(clientId, IPC_RMID, NULL) == -1)
            perror("msgctl");
        exit(EXIT_FAILURE);
    }

    /* Файл был успешно открыт сервером */

    totBytes = msgLen;
    for(numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = msgrcv(serverId, &resp, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1)
            perror("msgrcv");

        totBytes += msgLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);

    exit(EXIT_SUCCESS);
}