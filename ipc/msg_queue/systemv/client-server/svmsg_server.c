#include "svmsg_header.h"

/* Заголовок SIGCHILD */
static void grimReaper(int sig)
{
    int savedErrno;

    savedErrno = errno;                 /* waitpid() можнет изменить 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

/* Выполняется в дочернем процессе: обслуживает одного клиента  */
static void serverRequest(const struct requestMsg *req)
{
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Ошибка открытия: отправить текст ошибки */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE);
    }

    /* Передаем содержимое файла в сообщениях типа RESP_MT_DATA. Мы не
       проверяем на ошибки read () и msgsnd (), поскольку мы не можем 
       уведомить клиента. */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
            break;
    
    /* Отправляем сообщение типа RESP_MT_END для обозначения конца файла */

    resp.mtype = RESP_MT_END;
    msgsnd(req->clientId, &resp, 0, 0);
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    /* Создаем очередь сообщений сервера */

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                            S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1)
        perror("msgget");

    /* Устанавливаем обработчик SIGCHLD для получения завершенных потомков */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        perror("sigaction");

    /* Обрабатываем запросы каждый в отдельном дочернем процессе */

    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR)             /* Прервано обработчиком SIGCHLD? */
                continue;                   /* ... тогда перезапускаем msgrcv() */
            perror("msgrcv");
            break;
        }

        pid = fork();                       /* Создаем дочерний процесс */
        if (pid == -1) {
            perror("fork");
            break;
        }

        if (pid == 0) {
            serverRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Цикл родителя получает следующий запрос клиента */
    }

    /* Если msgrcv() или fork() завершились с ошибкой, 
       удаляем очередь сообщений сервера и завершаем программу */

    if (msgctl(serverId, IPC_RMID, NULL) == -1)
        perror("msgctl");
        
    exit(EXIT_SUCCESS);
}