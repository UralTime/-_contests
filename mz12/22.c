#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

volatile int flag = 0;

void 
handler_add(int s)
{
    flag = 0;
}

void
handler_mul(int s)
{
    flag = 1;
}

int 
main(void)
{
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler_add, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler_mul, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int res = 0, num;
    while (scanf("%d", &num) == 1) {
        if (flag) {
            res = (int) ((unsigned) res * (unsigned) num);
        } else {
            res = (int) ((unsigned) res + (unsigned) num);
        }
        printf("%d\n", res);
        fflush(stdout);
    }
    return 0;
}
