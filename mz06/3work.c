#include <stdlib.h>
#include <string.h>

struct Stack
{
    char array[1024 * 1024];
    int top;
};

void push(struct Stack *stack, char value)
{
    stack->top++;
    stack->array[stack->top] = value;
    stack->array[stack->top + 1] = '\0';
}

char pop(struct Stack *stack) 
{
    if (!stack->top) {
        return '/';
    }    
    char now = stack->array[stack->top]; 
    stack->array[stack->top] = '\0';
    stack->top--;
    return now;
}

char *remove_points(const char *str)
{
    char *without_one = calloc(1024 * 1024, sizeof(char));
    int i = 0;
    int j = 0;
    char *old = (char *) str;
    char *now;
    while (1) {
        now = strstr(old, "/./");
        if (!now) {
            while (old[j]) {
                without_one[i] = old[j];
                i++;
                j++;
            }
            break;
        } else {
            while (old + j < now) {
                without_one[i] = old[j];
                i++;
                j++;
            }
            old = now + 2;
            j = 0;
        }
    }
    without_one[i--] = '\0';
    if (i >= 2) {
        if (without_one[i] == '.' && without_one[i - 1] == '/') {
            without_one[i - 1] = '\0';
        }
    } else {
        if (i == 1 && without_one[i] == '.') {
            without_one[i] = '\0';
        }
    }
    int len = strlen(without_one);
    if (NULL == strstr(without_one, "/../")) {
        return without_one;
    }
    char *without_two = calloc(len, sizeof(char));
    struct Stack stack;
    stack.top = 0;
    stack.array[0] = '/';
    stack.array[1] = '\0';
    old = without_one;
    while (1) {
        i = 0;
        now = strstr(old, "/../");
        if (!now) {
            while(old[i]) {
                push(&stack, old[i]);
                i++;
            }
            break;
        }
        while (old + i < without_one + len && old + i != now) {
            push(&stack, old[i]);
            i++;
        }
        char c;
        while ((c = pop(&stack)) != '/') {}
        old = now + 3;
    }
    i = 0;
    j = 0;
    while (i <= stack.top + 1) {
        if (i != stack.top + 1 && stack.array[i] == '/' && stack.array[i + 1] == '/') {
            i++;
        }
        without_two[j] = stack.array[i];
        i++;
        j++;
    }
    len = strlen(without_two);
    if (len >= 3) {
        if(without_two[len - 1] == '.' && without_two[len - 2] ==  '.' && without_two[len - 3] == '/') {
            if (len - 3 != 0) {
                i = 4;
                while (without_two[len - i] != '/') {
                    i++;
                }
                if (len != i) {
                    without_two[len - i] = '\0';
                } else {
                    without_two[1] = '\0';
                }
            } else {
                without_two[1] = '\0';
            }
        }
    }
    free(without_one);
    return without_two;
}

char *relativize_path(const char *path1, const char *path2) 
{                                               
    char *without1 = remove_points(path1);
    char *without2 = remove_points(path2);   
    char *answer = calloc(1024 * 1024, sizeof(char));
    int i = 0, slashes = 0, near = 0;
    while (without1[i]) {
        slashes += without1[i++] == '/';
    }
    i = 0;
    if (slashes == 1) {
        if (!without2[1]) {
            answer[i] = '.';
            answer[i + 1] = '\0';
        } else {
            i = 1;
            while (without2[i]) {
                answer[i - 1] = without2[i];
                i++;
            }
            answer[i - 1] = '\0';
        }
    } else {
        while (without1[i] && without2[i] && without1[i] == without2[i]) {
            if (without2[i] == '/') {
                near = i;
            }
            i++;
        }
        if (!without2[i]) {
            if (!without1[i]) {
                i = near;
                i++;
                int j = 0;
                while (without2[i]) {
                    answer[j] = without2[i];
                    j++;
                    i++;
                }
                answer[j] = '\0';
            } else {   
                int count = 0;
                i = near + 1;
                while (without1[i]) {
                    if (without1[i] == '/') {
                        count++;
                    }
                    i++;
                }
                i = 0;
                if (count == 1 && without2[1]) {
                    answer[i] = '.';
                    answer[i + 1] = '\0';
                } else {
                    if (without2[1]) {
                        count--;
                    }
                    while (count) {
                        answer[i] = '.';
                        answer[i + 1] = '.';
                        if (count != 1) {
                            answer[i + 2] = '/';
                        } else {
                            answer[i + 2] = '\0';
                        }
                        i += 3;
                        count--;
                    }
                }
            }          
        } else if (!without1[i]) {    
                i--;
                while (i && without2[i] != '/') {
                    i--;
                }
                i++;
                int j = 0;
                while (without2[i]) {
                    answer[j] = without2[i];
                    j++;
                    i++;
                }
                answer[j] = '\0';
        } else {
            i = near + 1;
            int count = 0;
            int j = i;
            while (without1[j]) {
                if (without1[j] == '/') {
                    count++;
                }
                j++;
            }
            j = 0;
            while (count) {
                answer[j] = '.';
                answer[j + 1] = '.';
                answer[j + 2] = '/';
                j += 3;
                count--;
            }
            while (without2[i]) {
                answer[j] = without2[i];
                i++;
                j++;
            }
            answer[j] = '\0';
        }
    }
    free(without2);
    free(without1);
    return answer;
}
