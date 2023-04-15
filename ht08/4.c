#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

enum { BASIS = 10 };

typedef struct ArgStruct 
{
    char arg[64];
} ArgS;

int 
main(int argc, char* argv[])
{
    //загрузка библиотеки
    void *library = dlopen(argv[1], RTLD_LAZY);
    if (!library) {
        fprintf(stderr,"dlopen() error: %s\n", dlerror());
        return 1;
    };

    ArgS args;
    /*в эту структуру писать аргументы в порядке от первого к последнему при помощи memcpy. */
    int len = strlen(argv[3]), j = 0, number = 0;
    double dnumber = 0;
    for (int i = 1; i < len; i++) {
        switch (argv[3][i]) {
            case 'i':
                number = strtol(argv[3 + i], NULL, BASIS);
                memcpy(args.arg + j, &number, sizeof(number));
                j += sizeof(number);
                break;
            case 'd':
                dnumber = strtod(argv[3 + i], NULL);
                memcpy(args.arg + j, (char *)&dnumber, sizeof(dnumber));
                j += sizeof(dnumber);
                break;
            case 's':
                memcpy(args.arg + j, &argv[3 + i], sizeof(argv[3 + i]));
                j += sizeof(&argv[3 + i]);
                break;
            default:
                break;
        }
    }
/*Передаем в функцию из dlsym структуру по значению 
(то есть функция из dlsym имеет тип <выходной тип> f(struct arguments a);*/

    //выводим результат работы процедур
    switch (argv[3][0]) {
        case 'i': 
            printf("%d\n", ((int (*)(ArgS)) dlsym(library, argv[2])) (args));
            break;
        case 's': 
            printf("%s\n", ((char* (*)(ArgS)) dlsym(library, argv[2])) (args));
            break;
        case 'd':
            printf("%.10g\n", ((double (*)(ArgS)) dlsym(library, argv[2])) (args));
            break;
        default:
            ((void (*)(ArgS))dlsym(library, argv[2]))(args);
            break;
    }

    //закрываем библиотеку
    dlclose(library);
    return 0;
}
