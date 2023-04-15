#include <pthread.h>
#include <stdio.h>

enum { COUNT = 100 };

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
long long value = 0;

void *func(void *ptr)
{
    int mod = 1;
    if ((*(int*) ptr & 1)) mod = -1;
    for (int i = 0; i < 100000; ++i) {
        pthread_mutex_lock(&m);
        value += mod;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main()
{
    pthread_t ids[COUNT];
    int data[COUNT];
    int i;
    for (i = 0; i < COUNT; ++i) {
        data[i] = i;
        pthread_create(&ids[i], NULL, func, &data[i]);
    }
    for (int i = 0; i < COUNT; i++) {
        pthread_join(ids[i], NULL);
    }
    printf("%llu\n", value);
}
