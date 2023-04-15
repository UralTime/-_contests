#include <pthread.h>
#include <stdio.h>

enum { COUNT = 100 };

_Atomic long long value = 0;

//long long value = 0;

void *func(void *ptr)
{
    int mod = 1;
    if ((*(int*) ptr & 1)) mod = -1;
    for (int i = 0; i < 100000; ++i) {
       value += mod;
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
