#include <stdio.h>
#include <stdlib.h>

int satsum(int v1, int v2)
{
    int neg_one = ~0;
    int int_max = ~0U >> 1;
    int int_min = neg_one ^ int_max;

    if (((v1 > 0) && (v2 > int_max - v1)) || ((v2 > 0) && (v1 > int_max - v2)))
        return int_max;
    if (((v2 < 0) && (v1 < int_min - v2)) || ((v1 < 0) && (v2 < int_min - v1)))
        return int_min;
    return v1 + v2;
}
int main(int argc, char *argv[])
{
    argc = printf("%d\n", satsum(atoi(argv[1]), atoi(argv[2])));
}
    
