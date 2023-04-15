#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum { INIT_VAL = 0 };    // начальное значение, которым обмениваемся

// Пример на пинг-понг с ограничением максимального значения

int 
main(int argc, char *argv[])
{
    int nproc = strtol(argv[1], NULL, 10);
    key_t key = strtol(argv[2], NULL, 16);
    int max = strtoll(argv[3], NULL, 10);

    int semid = semget(key, nproc + 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        return 1;
    }
    semctl(semid, 0, SETALL, (unsigned short[]) { INIT_VAL, 1, 0 });

    for (int i = 1; i <= nproc; ++i) {
        if (!fork()) {
            while (1) {
                struct sembuf ops[1] =
                {
                    { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
                };
                // когда массив семафоров будет удален, semop завершится с кодом ошибки EIDRM
                if (semop(semid, ops, 1) < 0) {
                    _exit(0);
                }
                int val = semctl(semid, 0, GETVAL);
                printf("%d %d\n", i, val); 
                fflush(stdout);
                if (val == max) {
                    // при достижении максимального значения удаляем массив семафоров и завершаем работу
                    semctl(semid, 0, IPC_RMID);
                    _exit(0);
                }
                int getnum = (val != 0);
                for (int j = 0; j < 4; j++) getnum *= val % nproc;
                getnum %= nproc; getnum++;
                semop(semid, (struct sembuf[]) {{ .sem_num = getnum, .sem_op = 1, .sem_flg = 0}, {.sem_num = 0, .sem_op = 1, .sem_flg = 0}}, 2);
            }
        }
    }
    while (wait(NULL) > 0) {}
}
