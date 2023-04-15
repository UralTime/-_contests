#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    int randfile = open("/dev/urandom", O_RDONLY);
    unsigned long long value;
    read(randfile, &value, sizeof(value));
    close(randfile);
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "che%llu.c", value);
    char exec_path[PATH_MAX];
    snprintf(exec_path, sizeof(exec_path), "che%llu.o", value);
    FILE *file = fopen(path, "w"); 
    const char code[] = 
        "#include <math.h>\n"
        "double\n"
        "myfunc(double x)\n"
        "{\n"
        "\treturn "; 
    fprintf(file, "%s", code);
    if (ferror(file)) {
        return 1;
    }
    fprintf(file, "%s", argv[4]);
    if (ferror(file)) {
        return 1;
    }
    const char end[] = ";}";
    fprintf(file, "%s", end);
    if (ferror(file)) {
        return 1;
    }
    fclose(file);

    double a = strtod(argv[1], NULL);
    double b = strtod(argv[2], NULL);
    long long n = strtoll(argv[3], NULL, 10);
    
    int pid = fork();
    if (pid < 0) {
        return 1;
    } else if (!pid) {
        execlp("gcc", "gcc", "-fPIC", "-DPIC", "-c", path, NULL);
        printf("exec_error1\n");
        _exit(1);
    }
    wait(NULL);
    
    pid = fork();
    if (pid < 0) {
        return 1;
    } else if (!pid) {
        execlp("gcc", "gcc", "-fPIC", "-shared", exec_path, "-oche.so", "-lm", NULL);
        printf("exec_error2\n");
        _exit(1);
    }
    wait(NULL);
    void *lib = dlopen("./che.so", RTLD_LAZY);
    if (lib == NULL) { 
        printf("error\n"); 
        return 1;
    }
    double dx = (b - a) / n, x = a, sum = 0;
    for (long long i = 0; i < n; i++) {
        x = a + i * dx;    
        void *sym = dlsym(lib, "myfunc");
        sum += ((double (*)(double))sym)(x) * dx;   
    }
    printf("%.10g\n", sum);
    return 0;
}
