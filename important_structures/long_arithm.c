#include <stdio.h>
#include <ctype.h>

int longscanf (int *a, char *c) {
    int i = -1, tmp;
    while (isdigit(c[0] = getchar())) { // читаем первое до оператора, i - номер посл. знач. разряда
        if ((i == -1) && (c[0] == '0'))
            continue;
        i++;
        a[i] = c[0] - '0';
    }
    if (i == -1) {
        i = 0;
        a[0] = 0;
    }
    for (int t = 0; 2 * t < i; t++) { // делаем разворот, чтобы хранить младшие разряды в начале для операций в столбик
        tmp = a[t];
        a[t] = a[i - t];
        a[i - t] = tmp;
    }
    return i;
}

void addition (int *a, int *b, int *res, int m) {
    for (int t = 0; t <= m; t++) {
        res[t] += a[t] + b[t];
        if (res[t] > 9) { // складываем в столбик, в случае переполнения переносим 1 в новый разряд
            res[t + 1]++;
            res[t] -= 10;
        }
    }
}

void subtraction (int *a, int *b, int *res, int m) {
    for (int t = 0; t <= m; t++)
        if (a[t] < b[t]) { // занимаем из следующего разряда
            a[t + 1]--;
            res[t] = 10 + a[t] - b[t];
        }
        else
            res[t] = a[t] - b[t];
}

void multiplication(int *a, int *b, int *res, int i, int j) {
    for (int t = 0; t <= i; t++)
        for (int p = 0; p <= j; p++)
            res[t + p] += a[t] * b[p]; // умножаем по принципу столбика, только запись в соответствующий разряд
    // при таком подсчёте мы могли переполнить размер разряда более, чем в два раза
    for (int t = 0; t <= i + j; t++) {
        res[t + 1] += res[t] / 10; // поэтому для переноса воспользуемся делением.
        res[t] %= 10;
    }
}

int main (void) {
    int a[10001] = {0}, b[10001] = {0}, res[20002] = {0}; // числа храним по разрядам
    char c;
    int len1 = longscanf(a, &c);
    char op = c; //op - оператор
    int len2 = longscanf(b, &c);
    int maxlen = (len1 > len2) ? len1 : len2;
    if (op == '+') {
        addition(a, b, res, maxlen);
        if (res[maxlen + 1]) // если переполнили m-ый разряд, то вывести нужно на m+1 разряд больше
            maxlen++;
    }
    else if (op == '-') {
        int tmp;
        for (tmp = len1; tmp && a[tmp] == b[tmp]; tmp--);
        // если первое число меньше второго, то в столбик вычтем из второго первое и поставим перед результатом минус
        if ((len1 < len2) || (a[tmp] < b[tmp])) {
            printf("-");
            subtraction(b, a, res, maxlen);
        }
        else
            subtraction(a, b, res, maxlen);
        while (maxlen && !res[maxlen]) // убираем незначащие нули
            maxlen--;
    }
    else {
        multiplication(a, b, res, len1, len2);
        for (maxlen = len1 + len2; res[maxlen]; maxlen++); // результате может быть разрядов больше, чем в каждом из чисел
        while (maxlen && !res[maxlen]) // а могут быть незначащие нули
            maxlen--;
    }
    for (int t = maxlen; t >= 0; t--)
        printf("%d", res[t]);
    return 0;
}
