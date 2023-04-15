#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

enum { BASIS = 10 };

void work(int serial, int fdr, int fdw, int end)
{
    int v = 0;
    while (v < end - 1) {
        read(fdr, &v, sizeof(v));
        if (v < end) {
            printf("%d %d\n", serial, v); 
            fflush(stdout);
        }
        ++v;
        write(fdw, &v, sizeof(v));
    }   
}

int 
main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    int end = strtol(argv[1], NULL, BASIS);

    if (!fork()) {
        work(1, p1[0], p2[1], end);
        _exit(0);
    }
    if (!fork()) {
        work(2, p2[0], p1[1], end);
        _exit(0);
    }
    int v1 = 1;
    write(p1[1], &v1, sizeof(v1));
    while (wait(NULL) > 0);
    printf("Done\n");
    return 0;
}
