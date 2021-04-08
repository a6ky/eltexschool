#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVER_KEY 0x1aaaaaa1               /* Ключ для очереди сообщений сервера */

struct requestMsg {                         /* Запросы (от клиента к серверу) */
    long mtype;                             /* Неиспользуется */
    int clientId;                           /* ID очереди сообщений клиента */
    char pathname[PATH_MAX];                /* Файл для возврата */
};

/* REQ_MSG_SIZE вычисляет размер части 'mtext' структуры 'requestMsg'.
   Мы используем offsetof() для обработки возможности заполнения
   байтов между полями 'clientId' и 'pathname'. */

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - \
                      offsetof(struct requestMsg, clientId) + PATH_MAX)

#define RESP_MSG_SIZE 8192

struct responseMsg {                        /* Ответы (от сервера клиенту) */
    long mtype;                             /* Одно из значений RESP_MT_* ниже */
    char data[RESP_MSG_SIZE];               /* Содержимое файла / ответное сообщение */
};

/* Типы ответных сообщений, отправляемых от сервера клиенту  */

#define RESP_MT_FAILURE 1
#define RESP_MT_DATA    2
#define RESP_MT_END     3