#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <wait.h>

void run(char *cmd) {
            int status = 0;
            wait(&status);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                if (!fork()) {
                    execlp(cmd, cmd, NULL);
                    _exit(1);
                } else {
                    wait(NULL);
                    _exit(0);
                }
            } else {
                _exit(1);
            }
}

void check(char *prog, char *filef) {
    int file;
    if (!(file = open(filef, O_RDWR | O_CREAT | O_APPEND, 0666))) {
        _exit(1);
    } else {
        dup2(file, 1);
        close(file);
        execlp(prog, prog, NULL);
        _exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (!fork()) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        if (!fork()) {
            check(argv[1], argv[4]);
        } else {
            run(argv[2]);
        }
    } else {
        if (!fork()) {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            check(argv[3], argv[5]);
        } else {
            close(fd[0]);
            close(fd[1]);
            while (wait(NULL) != -1);
            return 0;
        }
    }
}
