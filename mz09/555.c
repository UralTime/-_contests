#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int 
main(void)
{
    int parent = getpid(), status;
    while (1) {
        int number;
        if (scanf("%d", &number) == EOF) {
            _exit(0);
        }
        int pid = fork();
        if (pid > 0) {
            wait(&status);
            if (WIFEXITED(status) && (status = WSTOPSIG(status))) {
                if (parent == getpid()) {
                    while (wait(NULL) > 0);
                    printf("-1\n");
                    fflush(stdout);
                    _exit(0);
                } else {
                    _exit(1);
                }
            } else {
                printf("%d\n", number);
                fflush(stdout);
                _exit(0);
            }
        } else if (pid < 0) {
            _exit(1);
        }        
    }
}
