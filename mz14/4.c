#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

enum 
{ 
    MAX_COUNT = 100,
    BASIS = 10
};

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int value = 0, num = 0, count;

void *
func(void *ptr)
{
    pthread_mutex_lock(&m);
    printf("%d %d\n", *(int *)ptr, value);
    fflush(stdout);
    num = value % count;
    if (num < 0) {
        num += count; //математический остаток
    }
    pthread_mutex_unlock(&m);
    return NULL;
}

int 
main(int argc, char *argv[])
{
    pthread_t ids[MAX_COUNT];
    count = strtol(argv[1], NULL, BASIS);
    int j = 0;
    while (scanf("%d", &value) == 1) {
        int old_num = num;
        pthread_create(&ids[num], NULL, func, &num);
        pthread_join(ids[old_num], NULL);
        j++;
    }
    return 0;
}
