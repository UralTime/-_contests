#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 1;
    }
    DIR *d = opendir(argv[1]);
    if (d == NULL) {
        fprintf(stderr, "failed to open '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }
    struct dirent *dd;
    unsigned long long cnt = 0;
    while ((dd = readdir(d))) {
        struct stat stb;
        char path[PATH_MAX];
        unsigned ok = 0;
        const char exe[] = ".exe";
        int EXE_LEN = sizeof(exe) - 1;
        if ((snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name)) < sizeof(path) 
                && !access(path, X_OK) && !stat(path, &stb) && S_ISREG(stb.st_mode)) {
            int len = strlen(path);
            ok = len >= EXE_LEN && !strcmp(&path[len - EXE_LEN], exe);
        }
        cnt += ok;
    }
    closedir(d);
    printf("%llu\n", cnt);
    return 0;
}
