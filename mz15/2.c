#include <stdio.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


enum 
{
    RAND_BORDERS = 10
};


void work(int iter_count, int gpsd, int count, int semid, int *ptr)
{
    srand(gpsd);
    for (int i = 0; i < iter_count; ++i) {
        int ind1 = rand() % count;
        int ind2 = rand() % count;
        int value = rand() % RAND_BORDERS;
        struct sembuf buf[] = 
        {
            { .sem_num = ind1, -1, 0},
            { .sem_num = ind2, -1, 0},
        };
        semop(semid, buf, ind1 == ind2 ? 1 : 2);
        //operation(ptr, ind1, ind2, value);
        buf[0].sem_op = 1;
        buf[1].sem_op = 1;
        semop(semid, buf, ind1 == ind2 ? 1 : 2);
    }
    _exit(0);
}


int
main(int argc, char *argv[])
{
    int count, key, nproc, iter_count;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);
    int *gpsd = calloc(nproc, sizeof(*gpsd));
    int *pids = calloc(nproc, sizeof(*pids));
    int semid = semget(key, count, 0600 | IPC_CREAT | IPC_EXCL);
    for (int i = 0; i < nproc; ++i) {
        sscanf(argv[5 + i], "%d", &gpsd[i]);
    }
    int *ptr = mmap(NULL, count * sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    for (int i = 0; i < count; ++i) {
        scanf("%d", ptr + i);
    }
    for (int i = 0; i < count; ++i) {
        struct sembuf buf[] = 
        {
            { .sem_num = i, 1, 0},
        };
        semop(semid, buf, sizeof(buf) / sizeof(buf[0]));
    }
    for (int i = 0; i < nproc; ++i) {
        if (!(pids[i] = fork())) {
            work(iter_count, gpsd[i], count, semid, ptr);
            _exit(0);
        }
    }
    for (int i = 0; i < nproc; ++i) {
        int status = 0;
        waitpid(pids[i], &status, 0);
    }
    for (int i = 0; i < count; ++i) {
        printf("%d\n", *(ptr + i));
        fflush(stdout);
    }
    semctl(semid, 0, IPC_RMID, (int) 0);
    munmap(ptr, count * sizeof(*ptr));
    free(pids);
    free(gpsd);
    return 0;
}
