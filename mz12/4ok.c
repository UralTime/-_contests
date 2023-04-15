#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

enum { BASIS = 10 };

volatile int usr_flag;

void 
work(int fd[2], int max, int ppid)
{
    FILE *filerd = fdopen(fd[0], "r");
    FILE *filewr = fdopen(fd[1], "w");
    int x = 1;
    if (ppid != -1) {
        if (x < max) {
            fprintf(filewr, "%d\n", x);
            fflush(filewr);
        }
        fprintf(filewr, "%d\n", getpid());
        fflush(filewr);
        kill(ppid, SIGUSR1);
    }   
    sigset_t s2;
    sigemptyset(&s2);
    int i = 0;
    while (x < max) {
        while (!usr_flag) {
            sigsuspend(&s2);
        }
        usr_flag = 0;
        if (fscanf(filerd, "%d", &x) == EOF) {
            break;
        }
        if (!i++) {
            if (ppid == -1) fscanf(filerd, "%d", &ppid);
        }
        if (x < max) {
            printf("%d %d\n", 2 - (x & 1), x);
            fflush(stdout);
        }
        ++x;
        fprintf(filewr, "%d\n", x);
        fflush(filewr);
        kill(ppid, SIGUSR1);
    }
    fclose(filerd);
    fclose(filewr);
    _exit(0);
}

void handler(int s)
{
    usr_flag = 1;
}

int 
main(int argc, char *argv[])
{
    volatile int max;
    int fd[2];
    int pid, pid1;
    pipe(fd);

    sigset_t s1;
    sigemptyset(&s1); // создаём пустое множество сигналов
    sigaddset(&s1, SIGUSR1); // добавляем туда SIGUSR1
    sigprocmask(SIG_BLOCK, &s1, NULL); // заблокировали сигналы в s1 (SIGUSR1)    
    max = strtol(argv[1], NULL, BASIS);
    pid = fork();
    if (!pid) {
        sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
        work(fd, max, -1);
    } else if ((pid1 = fork()) == 0) {
        sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
        work(fd, max, pid);
    } else {
        close(fd[0]);
        close(fd[1]);
        int status = 0;
        if (wait(NULL) == pid1) {
            kill(pid, SIGUSR1);
            waitpid(pid, &status, 0);
        } else {
            kill(pid1, SIGUSR1);
            waitpid(pid1, &status, 0);
        }
        printf("Done\n");
        fflush(stdout);
        return 0;
    }
}
