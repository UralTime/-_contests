#include <stdlib.h>
#include <stdio.h>

typedef struct List {
    int data;
    struct List *next;
} List;

List *create_list(int n) {
    List *head = malloc(sizeof(List));
    List *p = head;
    for (int i = 0; i < n; ++i) {
        p->data = i;
        if (i == n - 1) {
            p->next = NULL;
        } else {
            p->next = malloc(sizeof(List));
            p = p->next;
        }
    }
    return head;
}

void print_list(List *l) {
    for (List *p = l; p; p = p->next) {
        printf("%d ", p->data);
    }
    printf("\n");
}

void free_list(List *l) {
    for (List *p = l; p; ) {
        List *tmp = p;
        p = p->next;
        free(tmp);
    }
}

List *push_front(List *l, int data) {
    List *new_head = malloc(sizeof(List));
    new_head->data = data;
    new_head->next = l;
    return new_head;
}

List *push_back(List *l, int data) {
    List *new_tail = malloc(sizeof(List));
    new_tail->data = data;
    new_tail->next = NULL;
    if (l) {
        List *p;
        for (p = l; p->next; p = p->next);
        p->next = new_tail;
    } else {
        l = new_tail;
    }
    return l;
}
List * create_stack() {
    return NULL;
}
void push(List **head, int x) {
    *head = push_front(*head, x);
}
void pop(List **head) {
    List *lhead = *head;
    *head = (*head)->next;
    free(lhead);
}
int top(List *head) {
    return head->data;
}
int empty(List *head) {
    return head == NULL;
}
void free_stack(List *head) {
    free_list(head);
}

void reverse(void) {
    List * st = create_stack();
    char c;
    while ((c = getchar ()) != '.')
        push(&st, c);
    while (!empty(st)) {
        printf("%c", top(st));
        pop(&st);
    }
    free_stack(st);
}
//Look, it's a dragon!
//NO, it's Patrick!
//it's a bird?
//it's a plane?
//
int main(void) {
    reverse();
    return 0;
}