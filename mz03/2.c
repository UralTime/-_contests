#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int main(int argc, char **argv)
{
    long long sumplus = 0, summinus = 0;
    for (int i = 1; i < argc; ++i) {
        char *end;
        long long lval = strtol(argv[i], &end, 10);
        if (lval > 0) {
            sumplus += lval;
        } else {
            summinus += lval;
        }
    };
    printf("%lld\n %lld\n", sumplus, summinus);
    return 0;
}
