#include <stdio.h>
#include <stdlib.h>

#define UNVISITED 100000

typedef struct pair { // x и y координаты вершины на доске
    int x, y;
} pair; // тип данных, хранимых в очереди

typedef struct { // очередь на массиве, операции за О(1), но иногда реалок
    pair *items; // собственно массив
    int head; // индекс первого элемента в очереди
    int size; // количество элементов в очереди по факту
    int max_queue_size; // размер очереди
} queue;

// функции для работы с очередью
void init (queue *q) { // инициализация
    q->head = 0;
    q->size = 0;
    q->max_queue_size = 32;
    q->items = malloc(sizeof(pair) * q->max_queue_size);
}

void append (queue *q, pair value) { // добавление элемента
    if (q->head + q->size >= q->max_queue_size) {
        q->max_queue_size *= 2;
        q->items = realloc(q->items, sizeof(pair) * q->max_queue_size);
    }
    q->items[q->head + q->size] = value;
    q->size++;
}

void popleft (queue *q, pair *value) { // извлечение 1 элемента из очереди
    *value = q->items[q->head];
    q->head++;
    q->size--;
}

int empty (queue *q) { // очередь пуста
    return q->size == 0;
}

int main(void) {
    int n, x0, y0, x1, y1;
    scanf("%d", &n);
    scanf("%d %d %d %d", &x0, &y0, &x1, &y1);
    x0--; y0--; x1--; y1--;
    queue *q = malloc(sizeof(queue));
    init(q);
    pair p1, p2;
    p1.x = x0;
    p1.y = y0;
    int **dist = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        dist[i] = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dist[i][j] = UNVISITED;
    append(q, p1);
    dist[x0][y0] = 0; //расстояние до начала равно 0 (0 ходов)
    int k, u, v;
    while (!(empty(q))) { // алгоритм поиска в  ширину (bfs)
        popleft(q, &p1);
        for (int i = -2; i <= 2; i++) { // перебор хода коня по горизонтали
            if (!i)
                continue;
            k = abs(i) % 2 + 1;
            for (int j = k; j != -3 * k; j -= 2 * k) { // для j и -j (перебор хода коня по вертикали)
                u = p1.x + i;
                v = p1.y + j;
                if ((0 <= u) && (u < n) && (0 <= v) && (v < n) && (dist[u][v] == UNVISITED)) {
                    dist[u][v] = dist[p1.x][p1.y] + 1;
                    p2.x = u;
                    p2.y = v;
                    append(q, p2); // ещё не посещали клетку в обходе - обновляем расстояние и в очередь
                }
            }
        }
    }
    if (dist[x1][y1] != UNVISITED) // смогли дойти на каком-то шаге в обходе bfs
        printf("%d\n", dist[x1][y1]);
    else
        printf("-1");
    //подметаем за собой
    free(q->items);
    free(q);
    for (int i = 0; i < n; i++)
        free(dist[i]);
    free(dist);
    return 0;
}