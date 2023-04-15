#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int 
main(void) 
{
    int fds[2];
    pipe(fds);
    int sonpid = fork();
    if (sonpid < 0) {
        return 1;
    } else if (!sonpid) {
        int sonsonpid = fork();
        if (sonsonpid < 0) {
            return 1;
        } else if (!sonsonpid) {
            long long sum = 0;
            int number;
            close(fds[1]);
            while (read(fds[0], &number, sizeof(number)) == sizeof(number)) {
                sum += number;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        } else {
            close(fds[0]);
            close(fds[1]);
            waitpid(sonsonpid, NULL, 0);
            _exit(0);
        }
    } else {
        close(fds[0]);
        int num;
        while (scanf("%d", &num) > 0) {
            write(fds[1], &num, sizeof(num));
        }
        close(fds[1]);
        waitpid(sonpid, NULL, 0);
        return 0;
    }
}
