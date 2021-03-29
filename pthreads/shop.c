#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SHOP_COUNT 5
#define CUSTOMER_COUNT 3

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

int shop[SHOP_COUNT];
int need_customer[CUSTOMER_COUNT];
static pthread_mutex_t mtx[SHOP_COUNT] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

int get_rand_range(const int min, const int max)
{  
    return rand() % (max - min + 1) + min;
}

void init_shop() 
{
    int i;
    for (i = 0; i < SHOP_COUNT; i++)
        shop[i] = get_rand_range(950, 1050);
}

void init_customer()
{
    int i;
    for (i = 0; i < CUSTOMER_COUNT; i++)
        need_customer[i] = get_rand_range(4500, 5000);
}

void customer_log(int customer_index, int shop_index)
{
    printf("\n");
    printf("----------------- Customer Log -----------------\n");
    printf("\tПокупатель: %d\n", customer_index);
    printf("\tПосетил магазин %d\n", shop_index);
    printf("\tТекущая потребность: %d\n", need_customer[customer_index]);
    printf("------------------------------------------------\n");
    printf("\n");
}

void loader_log(int shop_index)
{
    printf("\n");
    printf("------------------ Loader Log ------------------\n");
    printf("Погрузчик загрузил 500 товаров в %d-й магазин\n", shop_index);
    printf("Количество товаров в %d-ом магазине: %d\n", shop_index, shop[shop_index]);
    printf("------------------------------------------------\n");
    printf("\n");    
}

void *customer_task(void *arg)
{   
    int i = *((int *) arg);
    int rand_i = get_rand_range(0, SHOP_COUNT - 1);

    while (need_customer[i] >= 0) {
        pthread_mutex_lock(&mtx[i]);

        if (shop[rand_i] > need_customer[i]) {
            shop[rand_i] -= need_customer[i];
            need_customer[i] -= shop[rand_i];
        } else {
            need_customer[i] -= shop[rand_i];
            shop[rand_i] = 0;
        }
        pthread_mutex_unlock(&mtx[i]);
        customer_log(i, rand_i);
        sleep(2);
    }

    return NULL;
}

void *loader_task(void *arg) 
{
    int i;
    while (1) {
        for (i = 0; i < SHOP_COUNT; i++) {
            pthread_mutex_lock(&mtx[i]);
            shop[i] += 500;
            pthread_mutex_unlock(&mtx[i]);
            loader_log(i);
            sleep(1);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    pthread_t customer[CUSTOMER_COUNT];
    pthread_t loader;
    int i, j, s;
    void *res;

    init_shop();
    init_customer();

    for (i = 0; i < CUSTOMER_COUNT; i++) {
        s = pthread_create(&customer[i], NULL, customer_task, &i);
        if (s != 0)
            handle_error_en(s, "pthread_create");
        sleep(1);
    }

    s = pthread_create(&loader, NULL, loader_task, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    for (j = 0; j < CUSTOMER_COUNT; j++) {
        s = pthread_join(customer[j], &res);
        if (s != 0)
            handle_error_en(s, "pthread_join");
    }

    s = pthread_join(loader, &res);
    if (s != 0)
        handle_error_en(s, "pthread_join");

    exit(EXIT_SUCCESS);
}