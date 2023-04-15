#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum 
{ 
    BYTES_IN_KIBIBYTE = 1024
};

int
main(int argc, char *argv[])
{
    long long sum = 0;
    for (int i = 1; i < argc; i++) {
        struct stat stb;
        if (lstat(argv[i], &stb) >= 0 && S_ISREG(stb.st_mode) && 
            stb.st_size % BYTES_IN_KIBIBYTE == 0 && stb.st_nlink == 1) {
                sum += stb.st_size; 
        }
    }
    printf("%lld\n", sum);
    return 0;
}
