#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    COUNT = 10000,
    BASIS = 10,
    RAND_RANGE = 10
};

pthread_mutex_t global = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m[COUNT];
int count, iter_count, nproc;

void *
func(void *ptrr)
{
    int **ptr = (int **) ptrr;
    for (int i = 0; i < iter_count; i++) {
        int ind1 = rand_r((unsigned int *)ptr[1]) % count,
        ind2 = rand_r((unsigned int *)ptr[1]) % count,
        value = rand_r((unsigned int *)ptr[1]) % RAND_RANGE;
        pthread_mutex_lock(&global);
        pthread_mutex_lock(&m[ind1]);
        if (ind1 != ind2) {
            pthread_mutex_lock(&m[ind2]);
        }
        pthread_mutex_unlock(&global);
        operation(ptr[0], ind1, ind2, value);
        pthread_mutex_unlock(&m[ind1]);
        if (ind1 != ind2) {
            pthread_mutex_unlock(&m[ind2]);   
        } 
    }
    return NULL;
}

int 
main(int argc, char *argv[])
{
    int gprn[COUNT];
    count = strtol(argv[1], NULL, BASIS);
    nproc = strtol(argv[3], NULL, BASIS);
    iter_count = strtol(argv[4], NULL, BASIS);
    for (int i = 0; i < nproc; ++i) {
        gprn[i] = strtol(argv[5 + i], NULL, BASIS);
    }

    int data[COUNT];
    for (int i = 0; i < count; ++i) {
        scanf("%d", &data[i]);
    }
    for (int i = 0; i < count; i++) {
        pthread_mutex_init(&m[i], NULL);
        pthread_mutex_unlock(&m[i]);
    }

    pthread_t ids[COUNT];
    for (int i = 0; i < nproc; ++i) {
        int **ptr = calloc(2, sizeof(*ptr));
        ptr[0] = data;
        ptr[1] = calloc(1, sizeof(*ptr[1]));
        *ptr[1] = gprn[i];
        pthread_create(&ids[i], NULL, func, ptr);
        free(ptr[1]);
        free(ptr);
    }
    for (int i = 0; i < nproc; i++) {
        pthread_join(ids[i], NULL);
    }
    for (int i = 0; i < count; i++) {
        printf("%d\n", data[i]); 
        fflush(stdout);
    }
    for (int i = 0; i < count; i++) {
        pthread_mutex_destroy(&m[i]);
    }
    return 0;
}
