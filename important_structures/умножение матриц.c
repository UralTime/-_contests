#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int k, n, p; // даны коэффициенты a1,a2,...,ak и первые значение рекуррентной посл-ти f1,f2,...,fk
    scanf("%d%d%d", &k, &n, &p); // требуется вычислить f{n}
    long long f[k], a[k]; // у меня будет нумерация с нуля, поэтому нужно вычислить f{n-1} член
    n--; // теперь f{n}
    for (int i = 0; i < k; i++)
        scanf("%lld", &f[i]); // тогда даны коэф-ты a0,...,a{k-1}
    for (int i = 0; i < k; i++)
        scanf("%lld", &a[i]); // и значения f0,...f{k-1}
    if (n < k) {
        printf("%lld", f[n] % p); // если это f0/.../f{k-1}, то мы его знаем, просто выводим по модулю
        return 0;
    }
    long long **m = malloc(k * sizeof(long long*)); // матрица, описанная в решении задачи в сообщении в тг
        /* имеем равенство
     f{n}         ( a0  a1 a2 ... a{k+1} a{k})       ( f{n-1} )
     f{n-1}   =   ( 1   0  0  ...   0      0)     *  ( f{n-2} )
     ...                 ...                            ...
     f{n-k}       ( 0   0  0  ...   1      0)        ( f{n-k-1} )
    */
    long long **ans = malloc(k * sizeof(long long*)); // матрица ответа это m в степени n-k-1
    for (int i = 0; i < k; i++) {
        m[i] = calloc(k, sizeof(long long));
        ans[i] = malloc(k * sizeof(long long));
        m[0][i] = a[i] % p;
    }
    for (int j = 0; j < k - 1; j++)
        m[j + 1][j] = 1;   // см. матрицу выше
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            if (i == j)
                ans[i][j] = 1; // сначала это единичная матрица
            else
                ans[i][j] = 0;
    long long **c = malloc(k * sizeof(long long*)); // просто вспомогательная матрица, в которой мы сохраняем имеющуюся матрицу ans перед умножением на m
    for (int i = 0; i < k; i++) {
        c[i] = malloc(k * sizeof(long long));
    }
    int pow = n + 1 - k;
    while (pow) {  // алгоритм возведения в степень, описанный в тг
        if (pow & 1) {
            // res *= a
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++)
                    c[i][j] = ans[i][j];
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++) {
                    ans[i][j] = 0;
                    for (int t = 0; t < k; t++)
                        ans[i][j] += ((c[i][t] % p) * (m[t][j] % p) % p);
                }
        }
        // a *= a
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                c[i][j] = m[i][j];
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++) {
                m[i][j] = 0;
                for (int t = 0; t < k; t++)
                    m[i][j] += ((c[i][t] % p) * (c[t][j] % p) % p);
            }
        pow >>= 1;
    }
    // перемножаем полученную матрицу в степени pow на вектор столбец f{k},...,f{1} и берём первый элемент столбца, т.е. f{n}
    long long answer = 0;
    for (int t = 0; t < k; t++) {
        answer += ((ans[0][t] % p) * (f[k - 1 - t] % p) % p);
        answer %= p;
    }
    printf("%lld", answer);
    // подчищаем за собой
    for (int i = 0; i < k; i++) {
        free(m[i]);
        free(ans[i]);
        free(c[i]);
    }
    free(m);
    free(ans);
    free(c);
    return 0;
}
