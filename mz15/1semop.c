#include <stdio.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


void work(uint32_t *ptr, int32_t maxval, int32_t sem, int32_t nproc, int32_t semid)
{
    while (1) {
        struct sembuf buf[] =
        {
            { .sem_num = sem, -1, 0},
        };
        semop(semid, buf, sizeof(buf) / sizeof(buf[0]));
        if (*ptr > maxval) {
            _exit(0);
        }
        printf("%d %d %d\n", sem, *ptr, *(ptr + 1));
        fflush(stdout);
        ++*ptr;
        if (*ptr > maxval) {
            struct sembuf buf2[] =
            {
                { .sem_num = 0, 1, 0},
            };
            semop(semid, buf2, sizeof(buf2) / sizeof(buf2[0]));
            _exit(0);
        }
        *(ptr + 1) = sem;
        uint32_t val = *ptr;
        uint32_t send_sem = val % nproc * val % nproc * val % nproc * val % nproc + 1;
        struct sembuf buf3[] =
        {
            { .sem_num = send_sem, 1, 0},
        };
        semop(semid, buf3, sizeof(buf3) / sizeof(buf3[0]));
    }
}


int
main(int argc, char *argv[])
{
    int nproc, key, maxval;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &maxval);
    int *pids = calloc(nproc, sizeof(*pids));
    int semid = semget(key, nproc + 1, 0600 | IPC_CREAT | IPC_EXCL);
    uint32_t *mem = mmap(NULL, 2 * sizeof(*mem), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANON, -1, 0);
    semctl(semid, 1, SETVAL, 1); 
    for (int i = 0; i < nproc; ++i) {
        if (!(pids[i] = fork())) {
            work(mem, maxval, i + 1, nproc, semid);
            _exit(0);
        }
    }
    struct sembuf buf[] =
    {
        { .sem_num = 0, -1, 0},
    };
    semop(semid, buf, sizeof(buf) / sizeof(buf[0]));
    for (int i = 1; i <= nproc; ++i) {
        struct sembuf buf2[] =
        {
            { .sem_num = i, 1, 0},
        };
        semop(semid, buf2, sizeof(buf2) / sizeof(buf2[0]));
        int status = 0;
        waitpid(pids[i - 1], &status, 0);
    }
    free(pids);
    munmap(mem, 2 * sizeof(*mem));
    semctl(semid, 0, IPC_RMID, (int) 0);
    return 0;
}
