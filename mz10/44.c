#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int 
main(int argc, char *argv[])
{
    char path[PATH_MAX];
    int rfd = open("/dev/urandom", O_RDONLY);
    if (rfd < 0) {
printf("pizdec\n");
        return 1;
    }
    unsigned long long val;
    if (read(rfd, &val, sizeof(val)) != sizeof(val)) {
printf("pizdec12\n");
        return 1;
    }
    char *dir = getenv("XDG_RUNTIME_DIR");
    if (!dir) {
        dir = getenv("TMPDIR");
    }
    if (!dir) {
        dir = "/tmp";
    }
    if (snprintf(path, sizeof(path), "%s/hack%llu.py", dir, val) >= sizeof(path)) {
printf("pizdec13\n");
        return 1;
    }
    if (close(rfd) < 0) {
printf("pizdec14\n");
        return 1;
    }
    int filedes = open(path, O_WRONLY | O_CREAT, 0700);
    FILE *file = fdopen(filedes, "w");
    const char s[] = "#! /usr/bin/python3\nimport os\nprint(1";    
    fprintf(file, "%s", s);
    if (ferror(file)) {
printf("pizdec15\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        fprintf(file, " * ");
        if (ferror(file)) {
printf("pizdec0\n");
            return 1;
        }
        fprintf(file, "%s", argv[i]);
        if (ferror(file)) {
printf("pizdec1\n");
            return 1;
        }
    }
    const char rm[] = ")\nos.remove(\"";
    fprintf(file, ")\nos.remove(\"");
    if (ferror(file)) {
printf("pizdec2\n");
        return 1;
    }
    fprintf(file, "%s", path);
    if (ferror(file)) {
printf("pizdec3\n");
        return 1;
    }
    fprintf(file, "\")");
    if (ferror(file)) {
printf("pizdec4\n");
        return 1;
    }
    if (fclose(file) < 0) {
printf("pizdec5\n");
        return 1;
    }
    execl(path, path, NULL);
printf("pizdec6\n");
    return 1;
} 
