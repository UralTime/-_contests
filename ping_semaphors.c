#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

enum { KEY = 0xdeadbeef };
enum { COUNT = 2 };
enum { MAX = 10 };       // максимальное значение
enum { INIT_VAL = 0 };    // начальное значение, которым обмениваемся

// Пример на пинг-понг с ограничением максимального значения

int 
main()
{
    int semid = semget(KEY, 3, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        return 1;
    }
    semctl(semid, 0, SETALL, (unsigned short[]) { INIT_VAL, 1, 0 });

    for (int i = 1; i <= COUNT; ++i) {
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
                printf("%d %d\n", i, val); fflush(stdout);
                if (val == MAX) {
                    // при достижении максимального значения удаляем массив семафоров и завершаем работу
                    semctl(semid, 0, IPC_RMID);
                    _exit(0);
                }
                semop(semid, (struct sembuf[]) {{ .sem_num = 3 - i, .sem_op = 1, .sem_flg = 0}, {.sem_num = 0, .sem_op = 1, .sem_flg = 0}}, 2);
            }
        }
    }
    while (wait(NULL) > 0) {}
}
