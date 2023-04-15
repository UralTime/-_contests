#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>

typedef struct DirInfo
{
    char path[2 * PATH_MAX], name[2 * PATH_MAX];
} DirInfo;

int 
comp(const void *a, const void *b)
{
    return strcasecmp((* (DirInfo **) a) -> name, (* (DirInfo **) b) -> name);
}

void 
scan(char *dir, char *root)
{
    DIR *d = opendir(dir);
    if (!d) return;
    struct dirent *dd;
    int pos = 0; int max_len = sizeof(pos) * sizeof(pos);
    DirInfo **paths = calloc(max_len, sizeof(DirInfo *)); 
    while ((dd = readdir(d))) {
        struct stat stb;
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name);
        if (lstat(path, &stb) >= 0 && S_ISDIR(stb.st_mode) 
                && strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            if (pos >= max_len) {
                max_len *= sizeof(max_len);
                paths = realloc(paths, max_len * sizeof(DirInfo*));
            }
            paths[pos] = calloc(1, sizeof(DirInfo));
            for (int i = 0; dd->d_name[i]; i++) paths[pos]->name[i] = dd->d_name[i];
            for (int i = 0; path[i]; i++) paths[pos]->path[i] = path[i];
            pos++;
        }
    }
    closedir(d);
    if (pos) {
        qsort(paths, pos, sizeof(DirInfo *), comp);
        for (int i = 0; i < pos; ++i) {
            DIR *curdir = opendir(paths[i]->path);
            if (curdir) {
                closedir(curdir);
                printf("cd %s\n", paths[i]->name);
                scan(paths[i]->path, root);
            }
        }
    }
    for (int i = 0; i < pos; ++i) {
        free(paths[i]);
    }
    free(paths);
    if (strcasecmp(dir, root)) printf("cd ..\n");
}

int 
main(int argc, char *argv[])
{
    scan(argv[1], argv[1]);
    return 0;                        
}
