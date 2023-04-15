#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    struct dirent *dd;
    while ((dd = readdir(d))) {
        struct stat stb;
        char path[PATH_MAX];
        if (snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name) >= sizeof(path)) {
            abort();
        }
        if (stat(path, &stb) < 0) continue; 
        printf("%lu: <%s>\n", dd->d_ino, dd->d_name);
    }
}

