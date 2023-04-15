#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void scan(const char *dir)
{
    DIR *d = opendir(dir);
    if (!d)
        fprintf(stderr, "failed to open '%s': %s\n", dir, strerror(errno));
        return;
    }
    struct dirent *dd;
    while ((dd = readdir(d))) {
        struct stat stb;
        char path[PATH_MAX];
        if (snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name) >= sizeof(path))
            abort();
        if (stat(path, &stb) < 0) continue; //lstat
        printf("%lu: <%s>, %lu, %lu\n", dd->d_ino, dd->d_name, stb.st_dev, stb.st_ino);
        if (S_ISDIR(stb.st_mode) && strcmp(dd->d_name, ".") && strcmp(dd->d_name, ".."))
          // add path to array
            if (a == u) {
                if (!(a *= 2)) a = 32;
                n = realloc(n, a * sizeof(n[0]));
            }
            n[u++] = strdup(dd->d_name);
        }

    }
    closedir(d);
    for (int i = 0; i < u; ++i) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir, n[i]);
        scan(path);
    }
    for (int i = 0; i < u; i++)
        free(n[i]);
    free(n);
}

int main(int argc, char *argv[])
{
    scan(argv[1]);
    return 0;                        
}
