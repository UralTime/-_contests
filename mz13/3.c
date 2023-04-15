#include <stdio.h>
#include <stdlib.h>

double ***transpose(double ***arr)
{
    if (arr == NULL) {
        return NULL;
    }
    int rows = 0;
    while (arr[rows] != NULL) {
        rows++;
    }
    int *lens = calloc(rows, sizeof(lens[0]));
    int maxlen = 0, ind_max = 0;
    for (int i = 0; i < rows; i++) {
        int k = 0;
        while (arr[i][k] != NULL) {
            k++;
        };
        lens[i] = k;
        if (k > maxlen) {
            maxlen = k;
            ind_max = i;
        }
    }
    double ***m = calloc(maxlen + 1, sizeof(m[0]));
    int *heights = calloc(maxlen + 1, sizeof(heights[0]));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < lens[i]; j++) {
            heights[j] = i + 1;
        }
    }
    heights[maxlen] = 0;
    for (int i = 0; i < maxlen + 1; i++) {
        m[i] = calloc(heights[i] + 1, sizeof(m[i][0])); //or rows
        m[i][heights[i]] = NULL;
    }
    for (int i = 0; i < maxlen; i++) {
        for (int j = 0; j < heights[i]; j++) {
            m[j][i] = calloc(1, sizeof(m[j][i][0]));
            m[j][i][0] = 0;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < lens[i]; j++) {
            m[j][i] = calloc(1, sizeof(m[j][i][0]));
            m[j][i][0] = arr[i][j][0];
        }
    }
    for (int i = 0; i < rows; i++) {
       free(m[maxlen][i]);
    }
    free(lens);
    free(heights);
    m[maxlen] = NULL;
    return m;
}

int main(){

    double ***x = calloc(1, sizeof(*x));
    x[0] = calloc(1, sizeof(**x));
    x[0][0] = calloc(1, sizeof(***x));
    x[0][0][0] = 0;
    x = transpose(x);
    printf("%lf\n", ***x);
}
