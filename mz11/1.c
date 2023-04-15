#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char *argv[]) 
{
    int fds[2];
    pipe(fds);
    pid_t father = fork();
    if (father < 0) {
        return 1;
    } else if (!father) { //son
        pid_t son = fork();
        if (son < 0) {
            return 1;
        } else if (!son) { //grandson
            pid_t grandson = fork();
            if (grandson < 0) {
                return 1;
            } else if (!grandson) { //great-grandson
                time_t t = time(NULL);
                close(fds[0]);
                write(fds[1], &t, sizeof(t));
                write(fds[1], &t, sizeof(t));
                write(fds[1], &t, sizeof(t));
                close(fds[1]);
                _exit(0);
            } else { //grandson
                close(fds[1]);
                waitpid(grandson, NULL, 0);
                time_t tgrand;
                read(fds[0], &tgrand, sizeof(tgrand));
                struct tm grantm;
                struct tm *grandtm = &grantm;
                grandtm = localtime_r(&tgrand, grandtm);
                close(fds[0]);
                printf("D:%02d\n", grandtm->tm_mday);
                fflush(stdout);
                _exit(0);
            }
        } else { //son
            close(fds[1]);
            waitpid(son, NULL, 0);
            time_t tson;
            read(fds[0], &tson, sizeof(tson));
            struct tm sotm;
            struct tm *sontm = &sotm;
            sontm = localtime_r(&tson, sontm);
            close(fds[0]);
            printf("M:%02d\n", sontm->tm_mon + 1);
            fflush(stdout);
            _exit(0);
        }
    } else { //father
        close(fds[1]);
        waitpid(father, NULL, 0);
        time_t tfather;
        read(fds[0], &tfather, sizeof(tfather));
        struct tm fathetm;
        struct tm *fathertm = &fathetm;
        fathertm = localtime_r(&tfather, fathertm);
        printf("Y:%04d\n", fathertm->tm_year + 1900);
        close(fds[0]);
        return 0;
    }
}    
