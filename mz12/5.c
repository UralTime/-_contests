#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

enum { SIGNAL_AMOUNT = 4 };

volatile sig_atomic_t flag = 0; 

int 
is_prime(int number)
{
    if (number <= 1) {
        return 0; // 1 is not prime also
    }
    for (unsigned i = 2; i < sqrt(number) + 1; i++) {
        if (number % i == 0) {
            return 0; // not prime
        }
    }
    return 1; // prime
}

void
int_signal(int sig)
{
    static int count = 0;
    if (++count != SIGNAL_AMOUNT) {
        flag = 1;
    } else {
        exit(0);
    }
}

void
term_signal(int sig)
{
    exit(0);
}

int
main(int argc, char *argv[])
{
    int low, high;
    scanf("%d %d", &low, &high);
    
    sigaction(SIGINT, &(struct sigaction){ .sa_flags = SA_RESTART, .sa_handler = int_signal}, NULL);
    sigaction(SIGTERM, &(struct sigaction){ .sa_flags = SA_RESTART, .sa_handler = term_signal}, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int last_prime = 0;
    for (int i = low; i < high; ++i) {
        if (is_prime(i)) {
            last_prime = i;
        }
        if (flag) {
            printf("%d\n", last_prime);
            fflush(stdout);
            flag = 0;
        }
    }
    
    printf("-1\n");
    fflush(stdout);
    return 0;
}
