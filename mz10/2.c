#include <sys/wait.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int mysys(const char *str)
{
    pid_t pid = fork();
    if (pid < 0) {
        return -1;  
    }
    if (!pid) {   
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status);
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    } else {
        return WEXITSTATUS(status);
    }
}
