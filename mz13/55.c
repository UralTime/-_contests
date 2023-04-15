#include <stdio.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGUSR1);
    sigaddset(&s, SIGUSR2);
    sigaddset(&s, SIGIO);
    sigaddset(&s, SIGALRM);
    sigprocmask(SIG_BLOCK, &s, NULL);
    struct signalfd_siginfo ssi;
    int filedes = signalfd(-1, &s, 0);
    int pid1 = fork();
    if (!pid1) {
        while (1) {
            char byte = 0;
            for (int i = 0; i < 8; ++i) {
                read(filedes, &ssi, sizeof(ssi));
                if (ssi.ssi_signo == SIGUSR2) {
                    byte |= (1 << i);
                } else if (ssi.ssi_signo == SIGIO) {
                    close(filedes);
                    _exit(0);
                }
                kill(ssi.ssi_pid, SIGALRM);
            }
            putchar(byte);
            fflush(stdout);
        }
    }
    int pid2 = fork();
    if (!pid2) {
        char byte;
        int out_fds = open(argv[1], O_RDONLY | O_CREAT, 0777);
        while (read(out_fds, &byte, sizeof(byte)) == sizeof(byte)) {
            for (int i = 0; i < 8; ++i) {
                if ((byte >> i) & 1) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }
                read(filedes, &ssi, sizeof(ssi));
            }
        }
        kill(pid1, SIGIO);
        _exit(0);
    }
    while (wait(NULL) > 0) {};
    return 0;
}
