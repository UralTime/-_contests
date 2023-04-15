#include <stdio.h>
#include <stdlib.h>

void build(int v, int l, int r, int *t) {
    if (l == r - 1) {
        t[v] = 0;
    return;
    }
    int m = (l + r) / 2;
    build(2 * v + 1, l, m, t);
    build(2 * v + 2, m, r, t);
    t[v] = t[2 * v + 1] + t[2 * v + 2];
}

void push(int v, int *c) {
    c[2 * v + 1] += c[v];
    c[2 * v + 2] += c[v];
    c[v] = 0;
}

void update(int v, int l, int r, int m, int *t, int *c) {
    t[v] = t[2 * v + 1] + t[2 * v + 2] + (m - l) * c[2 * v + 1] + (r - m) * c[2 * v + 2];
}

int ask(int v, int l, int r, int askl, int askr, int *t, int *c) {
    if (l >= askr || r <= askl)
        return 0;
    if (l >= askl && r <= askr)
        return t[v] + c[v] * (r - l);
    push(v, c);
    int m = (l + r) / 2;
    int ans = ask(2 * v + 1, l, m, askl, askr, t, c) + ask(2 * v + 2, m, r, askl, askr, t, c);
    update(v,l,r,m, t, c);
    return ans;
}

void change(int v, int l, int r, int askl, int askr, int val, int *t, int *c) {
    if (l >= askr || r <= askl)
        return;
    if (l >= askl && r <= askr) {
        c[v] += val;
        return;
    }
    push(v, c);
    int m = (l + r) / 2;
    change(2 * v + 1, l, m, askl, askr, val, t, c);
    change(2 * v + 2, m, r, askl, askr, val, t, c);
    update(v, l, r, m, t, c);
}

int main() {
    int *t = malloc(sizeof(int) * 400000); //strong defence from reject!
    int *c = malloc(sizeof(int) * 400000); //strong defence from reject!
    int n, m;
    scanf("%d %d", &n, &m);
    build(0, 0, n, t);
    for (int i = 0; i < m; i++) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            int posl, posr, val;
            scanf("%d %d %d", &posl, &posr, &val);
            change(0, 0, n, posl, posr, val, t, c);
            }
        else {
            int askl, askr;
            scanf("%d %d", &askl, &askr);
            int res = ask(0, 0, n, askl, askr, t, c);
            printf("%d\n", res);
        }
    }
}