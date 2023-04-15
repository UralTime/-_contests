#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum { MAXLEN = 512 };

void update (char *ans, char *cur_word, int *ans_len) {
    *ans_len = 0;
    for (int i = 0; cur_word[i] != '\0'; i++) {
        ans[i] = cur_word[i]; // копируем в ответ
        *ans_len += 1;
    }
    ans[*ans_len] = '\0';
}

int search(const char *SInputFile, char *word)
{
    FILE *file;
    if ((file = fopen(SInputFile, "r")) == NULL) {
        return -1; // файл не удалось открыть
    }
    char *cur_word = calloc(MAXLEN + 1, sizeof(char)); // текущее слово
    // строки длиной не более 512 символов и 1 на \0
    int ans_len = 0; // длина слова из ответа (чтобы не делать много strlen)
    int is_negative = 0; // флаг, есть ли отрицательные числа (0 => нет)
    char c; // текущий считываемый символ 
    int sign_word = 0; // работаем ли в данный момент со словом или это первый символ
    // храним знак слова (знак числа от этого слова)
    // если число 0, то знак пусть будет +
    int cur_len = 0; // длина этого слова
    int first_word = 1; // флаг, что слов пока не было
    do { // читаем до конца файла
        c = fgetc(file);
        if (isspace(c) || (c == EOF)) { // конец слова
            cur_word[cur_len] = '\0';
            if (first_word) {
                update(word, cur_word, &ans_len); // первое слово будет минимальным
                first_word = 0;
                is_negative = (cur_word[0] == '-');
            }
            // сравнение с ответом
            if (sign_word < 0) {

                if (is_negative) { // надо понять, какое число больше по модулю
                    // если длина одного больше длины другого, то и число больше
                    int we_have_new_ans = (cur_len > ans_len); 
                    //а вот если равны, то надо честно сравнивать посимвольно
                    if (cur_len == ans_len) {
                        for (int i = 1; i < cur_len; i++) { 
                        // начинаем с единицы, потому что первый символ это минусик
                            if (cur_word[i] > word[i]) {
                                we_have_new_ans = 1;
                                break;
                            }
                        }
                    }
                    if (we_have_new_ans) { // обновляем ответ
                        update(word, cur_word, &ans_len);
                    }

                } else { // наше число - новый ответ
                    update(word, cur_word, &ans_len);
                    is_negative = 1;
                }
            }

            else if (!is_negative) { // надо понять, какое число меньше по модулю
                // делаем почти то же самое
                int have_new_ans = (cur_len < ans_len);
                if (cur_len == ans_len) {
                    for (int i = 0; i < cur_len; i++) {
                        if (cur_word[i] < word[i]) {
                            have_new_ans = 1;
                            break;
                        }
                    }
                }
                if (have_new_ans) { // обновляем ответ
                    update(word, cur_word, &ans_len);
                }
            }
            sign_word = 0;
            cur_len = 0;

        } else { // не конец слова
            if (!sign_word) { // слова не было
                if (c == '-') { // ставим знак
                    sign_word = -1;
                } else { 
                    sign_word = 1;
                }
            } 
            cur_word[cur_len] = c;
            cur_len++;
        }
    } while (c != EOF);

    fclose(file); // не забываем освободить ресурсы
    return 0;
}

int main()
{
    char *word = calloc(MAXLEN + 1, sizeof(char));
    search("testsearch.txt", word);
    printf("%s\n", word);
}
