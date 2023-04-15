#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define UNVISITED LLONG_MAX

typedef struct pair { // x и y - координаты позиции на карте
    int x, y;
} pair;

typedef struct List {    // структура звена списка
    pair value;          // передаваемое значение
    struct List *next;
    struct List *prev;   // указатели на следующий и предыдущий элементы
} List;

typedef struct deque {  // двустороння очередь
    List *head;
    List *tail;     // указатели на начало и конец очереди
    int count;      // кол-во элементов в очереди
} deque;

void init (deque *q) { // инициализация дека
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
}

void push_back (deque *q, pair num) {
    List* element = malloc(sizeof(List)); // выделение памяти под новый элемент структуры
    element->value = num;       // добавляем значение в структуру
    element->next = NULL;
    element->prev = NULL;
    q->count++;
    if (NULL == q->head) {      // если список пуск
        q->head = element;      // т.к элемент единственный,
        q->tail = q->head;      // он является и хвостом и головой
    }
    else {
        element->prev = q->tail; // предыдущий элемент списка относительно добавленного будет последним(хвостом)
        q->tail->next = element; // следующий элемент за хвостом это добавляемый элемент списка
        q->tail = element;       // присваивание элементу статуса хвоста
    }
}

void push_front(deque *q, pair num) {
    List *element = malloc(sizeof(List));
    element->value = num;
    element->next = NULL;
    element->prev = NULL;
    q->count++;
    if (NULL == q->head) {
        q->head = element;
        q->tail = q->head;
    }
    else {
        element->next = q->head;      // следующий элемент за добавляемым элементом является хвост
        q->head->prev = element;      // перед головой находится добавляемый элемент
        q->head = element;           // присваивание элементу статуса головы
    }
}

void pop_front(deque *q, pair *value) {
    *value = q->head->value;
    if (q->head->next) {         // если в деке находится больше одного элемента
        List *element = q->head; // указываем на то, что будем использовать голову
        q->head = q->head->next; // присваиваем статус головы следующему за бывшей головой элементу
        q->head->prev = NULL;   // указываем на то, что перед головой пусто
        q->count--;        // уменьшаем размер дека
        free(element);     // удаляем бывшую голову
    }
    else {    // если элемент один в деке
        free(q->head);     // удаляем бывшую голову (она же и хвост)
        q->tail = NULL;     // указываем, что голова и хвост пусты
        q->head = NULL;
        q->count = 0;       // присваиваем ноль размеру дека
    }
}

int empty (deque *q) { // дек пустой
    return (q->count == 0);
}

void clear (deque *q) { // чистим мусор
    while (q->head)             // цикл: пока по адресу головы что-то лежит
    {
        q->tail = q->head->next;        //присваиваем статус хвоста следующему элементу, что лежит за головой
        free(q->head);           //удаляем первый элемент дека
        q->head = q->tail;             //указываем на то, что хвост принимает статус головы
    }
    free(q);
}

int main (void) {
    int n, m;
    scanf("%d %d", &n, &m);
    pair pstart, pfinish;
    scanf("%d %d %d %d", &(pstart.x), &(pstart.y), &(pfinish.x), &(pfinish.y));
    long long (*dist)[m] =  malloc(n * m * sizeof(long long)); // массив dist содержит кратчайшие расстояния до всех вершин
    int (*a)[m] =  malloc(n * m * sizeof(int));
    int (*id)[m] =  malloc(n * m * sizeof(int));
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
    deque *q = malloc(sizeof(deque));
    init(q);
    push_back(q, pstart);
    pair p1, p2;
    while (!empty(q)) {
        pop_front(q, &p1);
        id[p1.x][p1.y] = 2;
        for (int i = -1; i <= 1; i ++) // перебор соседей
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
    // чистим за собой
    free(id);
    free(a);
    free(dist);
    clear(q);
    return 0;
}