#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>

enum 
{ 
    BASIS = 10,
    PARAL_MODE = 0,
    POSLED_MODE = 1
};

int
exec_cmd(char *path, int mode)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return 1;
    }
    char cmd[PATH_MAX];
    fscanf(file, "%4096s", cmd);
    fclose(file);
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else if (mode == POSLED_MODE) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            return 0;
        }
    }
    return 1;
}

int 
main(int argc, char *argv[])
{
    int N = strtol(argv[1], NULL, BASIS);
    int cnt = 0;
    for (int i = 2; (i < N + 2) && (i < argc); i++) {
        exec_cmd(argv[i], PARAL_MODE);
    }
    int status;
    while (wait(&status) > 0) {
        cnt += WIFEXITED(status) && !WEXITSTATUS(status);
    }
    for (int i = N + 2; i < argc; i++) {
        cnt += !exec_cmd(argv[i], POSLED_MODE);
    }
    printf("%d\n", cnt);
    exit(0);
}
