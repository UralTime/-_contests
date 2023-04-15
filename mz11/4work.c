#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int rundup(int prev, int num, int argc, char *argv[], int fds_in_0, int fds_in_1)
{
    if (num >= argc) {
        return 0;
    }
    int fds_out[2];
    pipe(fds_out);
    int pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (!pid) { //son
        close(fds_out[0]);
        if (num > 1) {
            dup2(fds_in_0, 0);
            close(fds_in_0);
            close(fds_in_1);
        }
        if (num < argc - 1) {
            dup2(fds_out[1], 1);
        }
        close(fds_out[1]);
        execlp(argv[num], argv[num], NULL);
        return 1; //error   
    } else {
        if (num > 1) {
            close(fds_in_0);
            close(fds_in_1);
        }
        if (rundup(pid, num + 1, argc, argv, fds_out[0], fds_out[1])) {
            kill(pid, SIGKILL);
            return 1;
        } else {
            return 0;
        }
    }
}


int main(int argc, char *argv[])
{
    int ret = rundup(0, 1, argc, argv, 0, 0);
    while (wait(NULL) > 0) {};
    return ret;
}
