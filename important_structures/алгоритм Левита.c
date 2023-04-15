#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define UNVISITED LLONG_MAX

typedef struct pair { // x и y координаты позиции на карте
    int x, y;
} pair; // тип данных, хранимых в очереди

enum {
    max_size = 1100000
};
struct deque{
    pair items[max_size];
    int head, tail;
} deqq;
typedef struct deque deque;

void create_deq (deque *deq) {
    deq->head = max_size - 1;
    deq->tail = 0;
}

void push_back (deque *deq, pair val) {
    deq->items[deq->tail] = val;
    deq->tail = (deq->tail + 1) % max_size;
}

void push_front (deque *deq, pair val) {
    deq->items[deq->head] = val;
    deq->head = (deq->head + max_size - 1) % max_size;
}

pair get (deque *deq) {
    return deq->items[(deq->head + 1) % max_size];
}

void pop (deque *deq) {
    deq->head = (deq->head + 1) % max_size;
}

int empty (deque *deq) {
    return ((deq->head + 1) % max_size) == deq->tail;
}

int main (void) {
    int n, m;
    scanf("%d %d", &n, &m);
    pair pstart, pfinish, p1, p2;
    scanf("%d %d %d %d", &(pstart.x), &(pstart.y), &(pfinish.x), &(pfinish.y));
    long long dist[n][m]; // массив dist содержит кратчайшие расстояния до всех вершин
    int a[n][m], id[n][m];
    // для каждой вершины будем хранить, какому множеству она принадлежит:
    // 0 - если расстояние до неё равно бесконечности, то есть мы в ней не были
    // 1 - если вершина находится в очереди
    // 2 - если некоторый путь уже был найден, расстояние меньше бесконечности
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            dist[i][j] = UNVISITED;
            scanf("%d", &a[i][j]);
            id[i][j] = 0;
        }
    dist[pstart.x][pstart.y] = 0;
    deque *q = &deqq;
    create_deq(q);
    push_back(q, pstart);
    while (!empty(q)) {
        p1 = get(q);
        pop(q);
        id[p1.x][p1.y] = 2;
        for (int i = -1; i <= 1; i++) // перебор соседей
            for (int j = (i + 1) % 2; j >= -1; j -= 2) {
                p2.x = p1.x + i;
                p2.y = p1.y + j;
                // если сосед корректный, то есть не вышел за границы карты
                if ((0 <= p2.x) && (p2.x < n) && (0 <= p2.y) && (p2.y < m)) {
                    int len = a[p2.x][p2.y] - a[p1.x][p1.y];
                    if (len < 0)
                        len = -len;
                    if (dist[p2.x][p2.y] >
                        dist[p1.x][p1.y] + len) { // сосед ещё не просмотрен или можно улучшить расстояние
                        if (id[p2.x][p2.y] == 0) // сосед не был в очереди, расстояние не вычислено - добавляем
                            push_back(q, p2);
                        if (id[p2.x][p2.y] == 2) // расстояние до соседа было вычислено, а мы поняли, что можно улучшить
                            push_front(q, p2);
                        dist[p2.x][p2.y] = len + dist[p1.x][p1.y];
                        id[p2.x][p2.y] = 1;
                    }
                }
            }
    }
    printf("%lld", dist[pfinish.x][pfinish.y]);
    // подметаем мусор
    
    return 0;
}