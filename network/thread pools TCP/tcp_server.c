#include "tcp_header.h"
#include "myqueue.h"

/*
 * BACKLOG - максимальная длина, до которой может 
 *           расти очередь ожидающих соединений
 */
#define BACKLOG 5

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handleRequest(void *p_client_scoket)
{
    int cfd = *((int *) p_client_scoket);
    free(p_client_scoket);

    char    buf[BUF_SIZE];
    ssize_t num_read;

    for (;;) {
        num_read = recv(cfd, buf, BUF_SIZE, 0);

        if (num_read == 0)
            return 0;

        if (num_read == -1)
            handle_error("recv");

        printf("Receive message from client: %s\n", buf);

        if (send(cfd, buf, BUF_SIZE, 0) == -1)
            handle_error("send");
    }
    
    if (close(cfd) == -1)
        handle_error("close");
}

void *thread_function(void *args)
{
    for (;;) {
        pthread_mutex_lock(&mutex);
        int *pclient = dequeue();
        pthread_mutex_unlock(&mutex);

        if (pclient != NULL)
            handleRequest(pclient);
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in  svaddr;
    int                 i, sfd, cfd;
    int                 *pclient;

    for (i = 0; i < THREAD_POOL_SIZE; i++)
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);

    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd == -1)
        handle_error("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &(svaddr.sin_addr));
    svaddr.sin_port = htons(SERVER_PORT);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    if (listen(sfd, BACKLOG) == -1)
        handle_error("listen");
        
    for (;;) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
            handle_error("accept");

        pclient = malloc(sizeof(int));
        *pclient = cfd;
        
        pthread_mutex_lock(&mutex);
        enqueue(pclient);
        pthread_mutex_unlock(&mutex);
    }
    
    if (close(sfd) == -1)
        handle_error("close");

    return 0;
}