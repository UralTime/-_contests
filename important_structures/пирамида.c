#include <stdio.h>
#include <stdlib.h>

int cmp(int a, int b, int *predict) { // проверка а - родитель, b - сын
    if (*predict == 0) // уже точно не пирамида
        return 0;
    if (*predict == 1) // должна быть неубывающей
        return (a <= b);
    if (*predict == -1) // должны быть невозрастающей
        return (a >= b);
    // пока неяно, какая
    if (a < b)
        *predict = 1; // должна быть неубывающей
    if (a > b)
        *predict = -1; // должны быть невозрастающей
    return 1; // пока всё ок, проверим на следующем шаге
}

int main(void) {
    FILE *in = fopen("input.bin", "rb"), *out = fopen("output.bin", "wb");
    int ans = 2; // состояние 2 - пока неясно, пирамида это или нет и какая
    unsigned long maxsize = 128, size = 0; // size - реальное кол-во эл-тов в массиве, 4 МБ это примерно 4 миллиона элементов
    int *a = malloc(maxsize * sizeof(int));
    while (fread(a + size + 1, sizeof(int), 1, in) == 1) { // читаем массив, индексация с 1 (sorry)
        size++;
        if (size + 1 == maxsize) { // если не хватает размера - расширяем
            maxsize *= 2;
            a = realloc(a, maxsize * sizeof(int));
        }
    }
    for (unsigned long i = 1; 2 * i <= size; i++) {
        if (!ans) // уже не пирамида
            break;
        if (!cmp(a[i], a[2 * i], &ans))
            ans  = 0; // левый сын не прошёл проверку, значит, это не пирамида
        if (2 * i + 1 <= size)
            if (!cmp(a[i], a[2 * i + 1], &ans))
                ans = 0; // правый сын не прошёл проверку, значит, это не пирамида
    }
    if (ans == 2) // все эл-ты равны друг другу, по условию нужно вывести 1
        ans = 1;
    fwrite(&ans, sizeof(int), 1, out);
    free(a);
    fclose(in);
    fclose(out);
    return 0;
}
