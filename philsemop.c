#include <sys/sem.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

void work(int semid, int count, int serial, char *ptr)
{
    int n1 = serial;
    int n2 = (serial + 1) % count;
    while (1) {
        ptr[serial] = 'T';
        usleep(10000);
        ptr[serial] = 'W';
        // get forks
        semop(semid, (struct sembuf[])
                {
                    { .sem_num = n1, .sem_op = -1 },
                    { .sem_num = n2, .sem_op = -1 },
                }, 2);
        ptr[serial] = 'E';
        usleep(10000);
        // put forks
        semop(semid, (struct sembuf[])
            {
               { .sem_num = n1, .sem_op = 1 },
               { .sem_num = n2, .sem_op = 1 },
            }, 2);
    }
}


int main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, 10);
    int key = strtol(argv[2], NULL, 10);

    int semid = semget(key, count, IPC_CREAT | 0600);

    short *vals = calloc(count, sizeof(vals[0]));
    for (int i = 0; i < count; ++i) vals[i] = 1;
    semctl(semid, 0, SETALL, vals);

    char *ptr = mmap(NULL, count + 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    memset(ptr, '-', count);
    ptr[count] = 0;

    for (int i = 0; i < count; ++i) {
        if (!fork()) {
            work(semid, count, i, ptr);
            _exit(0);
        }
    }

    while (1) {
        usleep(10000);
        printf("%s\n", ptr);
    }
    
    while (wait(NULL) > 0) {};
}

