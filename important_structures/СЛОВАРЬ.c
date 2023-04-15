#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct WordCount {
    char *word;
    int count;
} WordCount;

typedef struct WordCountVector {
    WordCount *data;
    int n;
    int size;
} WordCountVector;

int add_word(char *word, WordCountVector *vector) {
    for (int i = 0; i < vector->n; ++i) {
        if (!strcmp(word, vector->data[i].word)) {
            free(word);
            vector->data[i].count++;
            return 0;
        }
    }
    if (vector->n == vector->size) {
        vector->size = (vector->size == 0) ? 1 : (vector->size * 2);
        vector->data = realloc(vector->data, sizeof(WordCount) * vector->size);
    }
    vector->data[vector->n].word = word;
    vector->data[vector->n].count = 1;
    vector->n++;
    return 1;
}

typedef struct CharVector {
    char *data;
    int n;
    int size;
} CharVector;

void add_char(char c, CharVector *vector) {
    if (vector->n == vector->size) {
        vector->size = (vector->size == 0) ? 1 : (vector->size * 2);
        vector->data = realloc(vector->data, sizeof(char) * vector->size);
    }
    vector->data[vector->n] = c;
    vector->n++;
}

char *to_string(CharVector *vector) {
    if (vector->n == 0) {
        return NULL;
    }
    add_char(0, vector);
    char *s = realloc(vector->data, sizeof(char) * vector->n);
    memset(vector, 0, sizeof(CharVector));
    return s;
}

int cmp(const void *a, const void *b) {
    const WordCount *wc1 = a;
    const WordCount *wc2 = b;
    if (wc1->count == wc2->count) {
        return strcmp(wc1->word, wc2->word);
    }
    return wc2->count - wc1->count;
}

int main(void) {
    //scan
    WordCountVector wc;
    CharVector s;
    memset(&wc, 0, sizeof(wc));
    memset(&s, 0, sizeof(s));
    while (1) {
        char c = getchar();
        if (c != EOF && isalpha(c)) {
            add_char(tolower(c), &s);
        } else {
            char *word = to_string(&s);
            if (word) {
                add_word(word, &wc);
            }
        }
        if (c == EOF) {
            break;
        }
    }

    //sort
    qsort(wc.data, wc.n, sizeof(WordCount), cmp);

    //print
    for (int i = 0; i < wc.n; ++i) {
        printf("%s %d\n", wc.data[i].word, wc.data[i].count);
        free(wc.data[i].word);
    }
    free(wc.data);

    return 0;
}