#include <stdlib.h>
#include <stdio.h>

enum { BASIS = 10 };

int 
main(int argc, char *argv[])
{
    int cache_size = strtol(argv[2], NULL, BASIS), block_size = strtol(argv[3], NULL, BASIS);
    int block_count = cache_size / block_size;
    int *cash = calloc(block_count, sizeof(cash[0]));
    if (cash == NULL) {
        return 1;
    }
    for (int i = 0; i < block_count; i++) {
        cash[i] = -1;
    }
    int ADDR, size, value;
    char mode, data_i;
    int misses = 0;
    while (scanf("%c%c %x %d %d\n", &mode, &data_i, &ADDR, &size, &value) != EOF) {
        int block_num = ADDR / block_size;
        int cache_block_num = block_num % block_count;
        if (mode == 'R') {
            misses += ((block_num != cash[cache_block_num]) && (cash[cache_block_num] != -1));
        }
        cash[cache_block_num] = block_num;
    } 
    printf("%d\n", misses);
    return 0;
}
