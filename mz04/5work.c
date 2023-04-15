#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc < 2)
        return 0;
    int file = open(argv[1], O_RDWR);
    if (!file)
        return 0;
    long long cur_num = 0, min_num = LLONG_MAX;
    int min_num_pos = 0, i = 0;
        for (i = 0; read(file, &cur_num, 8) > 0; i++) {
            if(cur_num < min_num) {
                if (cur_num == LLONG_MIN)
                    return 0;
                min_num = cur_num;
                min_num_pos = i;
            }
        }
    if (!i)
        return 0;
    min_num *= -1;
    lseek(file, min_num_pos * 8, SEEK_SET);
    write(file, &min_num, 8);
    close(file);
    return 0;
}
