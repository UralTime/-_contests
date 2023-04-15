#include <stdio.h>
#include <string.h>

int prefix_func(char *s) {
    int m = strlen(s);
    int p = 0, pi[20002] = {0};
    for (int i = 1; i < m; i++) {
        while ((p) && (s[p] != s[i]))
            p = pi[p - 1];
        if (s[p] == s[i])
            p++;
        pi[i] = p;
    }
    return pi[m - 1];
}

int main(void) {
    char s1[20002], s2[20002];
    fgets(s1, 10002, stdin);
    fgets(s2, 10002, stdin);
    int n = strlen(s1) - 1, m = strlen(s2) - 1;
    s1[n] = ' ';
    s2[m] = ' ';
    printf("%d %d", prefix_func(strncat(s1, s2, m)), prefix_func(strncat(s2, s1, n)));
    return 0;
}
