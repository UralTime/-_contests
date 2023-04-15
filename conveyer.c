#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fds[2];
    pipe2(fds, O_CLOEXEC);
    if (!fork()) {
        dup2(fds[1], 1);
        execlp(argv[1], argv[1], NULL);
    }
    if (!fork()) {
        dup2(fds[0], 0);
        execlp(argv[2], argv[2], NULL);
    }
    close(fds[0]); close(fds[1]);
    while (wait(NULL) > 0);
} 
