#include <stdio.h>

int
main(void)
{
    char filename[4100];
    unsigned long long cnt_num[10] = {0};

    if (fgets(filename, sizeof(filename), stdin)) {

        for (int i = 0; filename[i]; i++)
            if (filename[i] == '\n' || filename[i]=='\r') {
                filename[i] = 0;       
                break;
            }

        if (filename[0]) {
            FILE *file = fopen(filename, "r+");            
            if (file) { 
                char cur_char[50000];
                while (fgets(cur_char, sizeof(cur_char), file))
                    for (int i = 0; i < 50000 && cur_char[i]; i++)
                        if (cur_char[i] >= '0' && cur_char[i] <= '9')
                            cnt_num[cur_char[i] - '0']++;
                fclose(file);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        printf("%d %llu\n", i, cnt_num[i]);
    }
    return 0;
}
