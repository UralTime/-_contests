#include <stdio.h>
#include <stdlib.h>

typedef struct List { // Двусвязный список
    int data;
    struct List *next, *prev;
} List;

List *create_list(int n) { // создание списка с числами 1, ..., n
    List *head = malloc(sizeof(List));
    List *p = head;
    for (int i = 1; i <= n; ++i) {
        p->data = i;
        if (i == n) {
            p->next = head;
            head->prev = p; // список закольцован
        } else {
            p->next = malloc(sizeof(List));
            p->next->prev = p;
            p = p->next;
        }
    }
    return head;
}

void print_list(List *l, FILE *out) { // выводим эл-ты списка
    List *p = l;
    do {
        fprintf(out, "%d ", p->data);
        p = p->next;
    } while (p != l);
}

void free_list(List *l, int n) { // чистим за собой
    List *p = l;
    for (int i = 1; i < n; i++) {
        List *tmp = p;
        p = p->next;
        free(tmp);
    }
    free(p);
}

int main(void) {
    FILE *in, *out;
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    int N, M, left, right;
    fscanf(in, "%d %d", &N, &M);
    List *l = create_list(N);
    List *cur = l;
    List **point = malloc((N + 1) * sizeof(List*));
    for (int i = 1; i <= N; i++) {
        point[i] = cur; // point[i] - указатель на эл-т списка со значением i
        cur = cur->next;
    }
    for (int i = 0; i < M; i++) {
        fscanf(in, "%d %d", &left, &right); // приходит запрос переставить
        //right = l->prev->data;
        if (point[left] == l)
            continue;
        point[left]->prev->next = point[right]->next; // правый сосед leftа теперь не left, а правый сосед rightа
        point[right]->next->prev = point[left]->prev; // соотв. левый сосед правого соседа rightа теперь левый сосед leftа
        l->prev->next = point[left]; // после левого соседа головы (то есть хвоста) теперь left
        point[left]->prev = l->prev; // соотв. левый сосед leftа это хвост
        l->prev = point[right]; // перед головой встаёт right
        point[right]->next = l; // соотв. правый сосед rightа это голова
        l = point[left]; // теперь список начинается с leftа (это новая голова)
    }
    print_list(l, out);
    // убираем то, что натворили, сворачиваем файлы
    free_list(l, N);
    free(point);
    fclose(out);
    fclose(in);
    return 0;
}
