#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <assert.h>

int cnt = 0;
int max = 100;
int pid;
int pipefd[2];

void f(int s) {
    if (cnt < max) {
        read(pipefd[0], &cnt, sizeof(cnt));
        printf("%d %d\n", getpid(), cnt);
        cnt++;
        write(pipefd[1], &cnt, sizeof(cnt));
        kill(pid, SIGUSR1);
    } else {
        if (pid == getppid()) {
            close(pipefd[0]);
            close(pipefd[1]);  
            _exit(0);
        }
        kill(pid, SIGUSR1);
        signal(SIGUSR1, SIG_DFL);
    }
}
int main(void) {
    signal(SIGUSR1, f);
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        pid = getppid();
        write(pipefd[1], &cnt, sizeof(cnt));
        kill(pid, SIGUSR1);
        while (1) sleep(1);
    } else {
        int status;
        while (wait(&status) == -1) {
            sleep(1);
        }
        close(pipefd[0]);
        close(pipefd[1]);
    }
    return 0;
}