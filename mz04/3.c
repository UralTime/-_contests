#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

enum CONSTANTS
{
    SIKS_IN_GAL = 17,
    KNATS_IN_SIK = 29,
    BITS_IN_BYTE = 8,
    LEN_OF_NAME = 16
};

int
main(int argc, char *argv[])
{
    int max1 = INT_MIN, max2 = INT_MIN;
    char flag1 = 0, flag2 = 0;
    for (int i = 1; i < argc; i++) {
        int file = open(argv[i], O_RDONLY);
        if (file == -1) {
            printf("open error");
            return 1;
        }
        char name[LEN_OF_NAME];
        while (read(file, name, sizeof(name)) == sizeof(name)) {
            unsigned num = 0;
            unsigned char buf[sizeof(num)];
            if (read(file, buf, sizeof(buf)) != sizeof(buf)) {
                return 1;
            }
            for (int j = 0; j < sizeof(num); j++) {
                unsigned temp = (unsigned) buf[j];
                num += (temp << ((sizeof(num) - 1 - j) * BITS_IN_BYTE));
            }    
            int number = (int) num;
            if (!flag1) {
                flag1 = 1;
                max1 = number;
            } else if (number > max1) {
                max2 = max1;
                flag2 = 1;
                max1 = number;
            } else if (number != max1 && number >= max2) {
                max2 = number;
                flag2 = 1;
            }
        }
        if (close(file) == -1) {
            return 1;
        }
    }
    if (!flag2) {
        return 0;
    }
    if (max2 < 0) {
        printf("-");
        if (max2 == INT_MIN) {
            long long maxim2 = -(long long) INT_MIN;
            printf("%lldg%02llds%02lldk\n", maxim2 / KNATS_IN_SIK / SIKS_IN_GAL,
                (maxim2 / KNATS_IN_SIK) % SIKS_IN_GAL, maxim2 % KNATS_IN_SIK);
            return 0;
        }
        max2 = -max2;
    }
    printf("%dg%02ds%02dk\n", max2 / KNATS_IN_SIK / SIKS_IN_GAL, 
        (max2 / KNATS_IN_SIK) % SIKS_IN_GAL, max2 % KNATS_IN_SIK);
    return 0;
}
