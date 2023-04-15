#include <stdio.h>

int recursion(int prev) {
    char c = getchar();
    if (c == ' ')
        return (prev != -1)? prev: recursion(-1);
    if (c == '\n')
        return prev;
    if (c == '-')
        return -recursion(0);
    if (c == '*') {
        int x = recursion(-1);
        int y = recursion(-1);
        return x * y;
    }
    if (c == '/') {
        int x = recursion(-1);
        int y = recursion(-1);
        return x / y;
    }
    return (prev != -1)? recursion(10 * prev + c - '0'): recursion(c - '0');
}

int main(void) {
    int result = recursion(-1);
    printf("%d", result);
    return 0;
}
