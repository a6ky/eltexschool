#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define THREADS_COUNT 1000

static int glob = 0;
// static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void * arg) 
{
    int loops = *((int *) arg);
    int i, loc;

    for (i = 0; i < loops; i++) {
        // pthread_mutex_lock(&mtx);
        loc = glob;
        loc++;
        glob = loc;
        // pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t[THREADS_COUNT];
    int loops = 10000;
    int i, j;
    void *res;

    for (i = 0; i < THREADS_COUNT; i++)
        pthread_create(&t[i], NULL, thread_func, &loops);
    
    for (j = 0; j < THREADS_COUNT; j++)
        pthread_join(t[j], &res);

    printf("glob: %d\n", glob);

    exit(EXIT_SUCCESS);
}

