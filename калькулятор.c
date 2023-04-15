#include <stdio.h>
#include <ctype.h>

int main(void) {
    FILE *in, *out;
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    long long cur = -1, res = -1; //в cur храним текущее набираемое до пробела число, res - ответ
    char op = 0; //op - операция +-, s-считываемый символ
    char *s;
    fscanf(in, "%s", s);
    do {
        if (isdigit(s))
            if (op && (res != -1)) //если есть операция, то считываем в cur, иначе это наше первое число на входе
                cur = s - '0' + ((cur != -1) ? (cur * 10) : 0);
            else {
                op = 0;
                res = s - '0' + ((res != -1) ? (res * 10) : 0);
            }
        else {
            if (op && (res != -1) && (cur != -1)) { //cur число закончилось и можем делать операцию
                if (op == '+')
                    res += cur;
                if (op == '-')
                    res -= cur;
                cur = -1; // обнвляем значения параметров перед набором нового числа
                op = 0;
            }
            if (!op && ((s == '+') || (s == '-') || (s == '*') || (s == '/'))) // операции никакой не было и вот она
                op = s;
        }
    } while (fscanf(in, s)); // когда s=EOF тоже отработали, чтобы сделать последнюю операцию
    fprintf(out, "%lld", res);
    fclose(out);
    fclose(in);
    return 0;
}