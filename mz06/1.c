#include <time.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum 
{ 
    USEC_IN_SEC = 1000000,
    BASE = 10
};

int
main(int argc, char *argv[])
{
    if (argc <= 5)
        return 1;
    struct timeval t = {};

    char *eptr = NULL;
    errno = 0;
    t.tv_sec = strtol(argv[1], &eptr, BASE);
    if (errno || *eptr || eptr == argv[1]) {
        return 1;
    }

    eptr = NULL;
    errno = 0;
    t.tv_usec = strtol(argv[2], &eptr, BASE);    
    if (errno || *eptr || eptr == argv[2]) {
        return 1;
    }

    eptr = NULL;
    errno = 0;
    double alpha = strtod(argv[3], &eptr);
    if (errno || *eptr || eptr == argv[3]) {
        return 1;
    }

    eptr = NULL;
    errno = 0;
    int k = (int) strtol(argv[4], &eptr, BASE);
    if (errno || *eptr || eptr == argv[4]) {
        return 1;
    }
    
    eptr = NULL;
    errno = 0;
    unsigned s = (unsigned) strtol(argv[5], &eptr, BASE);
    if (errno || *eptr || eptr == argv[5]) {
        return 1;
    }
 
    srand(s);
    for (int i = 0; i < k; i++) {
        double u = (double) rand() / RAND_MAX; 
        double p = (double) (-log(u) / alpha);
        t.tv_usec += ((long) p) % USEC_IN_SEC;
        t.tv_sec += ((long) p) / USEC_IN_SEC;
        t.tv_sec += t.tv_usec / USEC_IN_SEC;
        t.tv_usec %= USEC_IN_SEC;
        printf("%ld %ld\n", t.tv_sec, t.tv_usec);
    }
    return 0;
}
