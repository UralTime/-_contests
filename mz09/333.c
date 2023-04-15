#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

enum
{
    STRING_AMOUNT = 3,
    STRING_LENGTH = 8,
    DECIMAL = 10
};

int
main(void)
{
    char string[STRING_LENGTH + 1];
    for (int i = 1; i <= STRING_AMOUNT; i++) {
        int pid = fork(); //paral 
        if (pid < 0) {
            return 1;
        } else if (pid == 0) {
            read(0, string, sizeof(string) - 1);
            string[STRING_LENGTH] = 0;
            int number = strtol(string, NULL, DECIMAL);
            printf("%d %d\n", i, number * number);
            fflush(stdout);
            _exit(0);
        }
    }
    while (wait(NULL) > 0) {}; //father is dying last
    return 0;
}
