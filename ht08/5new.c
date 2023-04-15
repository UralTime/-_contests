#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>

enum 
{
    FIRST_SIZE = 16,
    REALLOC_MUL = 4
}; //why not

int 
comp(const void *a, const void *b) //arg to qsort
{
    return strcasecmp(*(char **) a, *(char **) b);
}

void 
alphabet_dfs(char *dir, char *root)
{
    DIR *d = opendir(dir);
    if (d == NULL) { //dir didn't open
        return;
    }
    struct dirent *dd;
    int cnt = 0, max_size = FIRST_SIZE;
    char **paths = calloc(max_size, sizeof(*paths)); 
    char path[PATH_MAX];
    while ((dd = readdir(d))) {
        struct stat stb;
        if (snprintf(path, sizeof(path), "%s/%s", dir, dd->d_name) < sizeof(path) && lstat(path, &stb) == 0 
                && S_ISDIR(stb.st_mode) && strcmp(dd->d_name, ".") && strcmp(dd->d_name, "..")) {
            if (cnt >= max_size) { // we need to realloc
                max_size *= REALLOC_MUL; // lets's realloc by REALLOC_MUL
                paths = realloc(paths, max_size * sizeof(*paths));
            }   
            paths[cnt] = strdup(dd->d_name); 
            //paths[cnt] = calloc(...) and snprintf(paths[cnt],dd->d_name)
            cnt++;
        }
    }
    closedir(d);
    if (cnt) {
        qsort(paths, cnt, sizeof(*paths), comp);
        for (int i = 0; i < cnt; ++i) {
            if (snprintf(path, sizeof(path), "%s/%s", dir, paths[i]) < sizeof(path)) {
                DIR *curdir = opendir(path);
                if (curdir) {
                    closedir(curdir);
                    printf("cd %s\n", paths[i]);
                    alphabet_dfs(path, root); // go deeper
                }
            }
        }
    }
    // clean the trash after our code
    for (int i = 0; i < cnt; ++i) {
        free(paths[i]);
    }
    free(paths);
    if (strcasecmp(dir, root)) { // come back
        printf("cd ..\n");
    }
}

int 
main(int argc, char *argv[])
{
    alphabet_dfs(argv[1], argv[1]); // go deeper
    return 0;                        
}
