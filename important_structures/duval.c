#include <stdio.h>
#include <string.h>

int main(void) {
    char s[4000001], s1[2000001];
    fgets(s1, 2000001, stdin);
    int n = strlen(s1) - 1;
    for (int i = 0; i < n; i++) {
        s[i] = s1[i];
        s[i+n] = s1[i];
    }
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while ((j < 2 * n) && (s[k] <= s[j])) {
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        while (i <= k)
            i += j - k;
    }
    char s2[2000001];
    int w = 0;
    for (int q = ans; q < n; q++) {
        s2[w] = s1[q];
        w++;
    }
    for (int q = 0; q < ans; q++) {
        s2[w] = s1[q];
        w++;
    }
    s2[w] = '\0';
    printf("%s", s2);
    return 0;
}
