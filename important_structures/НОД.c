#include <stdio.h>

int gcd(int x, int y) {
    return y? gcd(y, x % y): x;
}

int main(void) {
    int a, b, c, d;
    scanf("%d%d%d%d", &a, &b, &c, &d);
    printf("%d", gcd(gcd(a, b), gcd(c, d)));
    return 0;
}
