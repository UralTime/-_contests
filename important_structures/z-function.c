#include <stdio.h>
#include <string.h>

void z_function (char s[]) {
    int n = strlen(s);
    int z[1000000] = {0};
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r)
            z[i] = (r - i + 1 < z[i - l]) ? (r - i + 1) : z[i - l];
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
        if (z[i] + i == n)
            printf("%d ", i);
    }
    printf("%d", n);
}

int main(void) {
    char s[1000001];
    scanf("%1000000s", s);
    z_function (s);
    return 0;
}
