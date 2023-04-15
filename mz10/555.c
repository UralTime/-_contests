#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    int randfile = open("/dev/urandom", O_RDONLY);
    unsigned value;
    read(randfile, &value, sizeof(value));
    close(randfile);
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "/tmp/prog%u.c", value);
    char exec_path[PATH_MAX];
    snprintf(exec_path, sizeof(exec_path), "/tmp/prog%u", value);
    int file = open(path, O_RDWR | O_CREAT | O_TRUNC, 0777);
    int file2 = open(exec_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
    char code[] = "#include <stdio.h>\n#include <stdlib.h>\nint main(int argc, char*argv[])\n{char *reject=\"reject\"; char *summon=\"summon\";\nchar *disqualify=\"disqualify\";int x = strtol(argv[1], NULL, 10);\nprintf(\"%s\\n\", ";
    write(file, code, strlen(code));
    write(file, argv[1], strlen(argv[1]));
    char end[] = "); return 0;}";
    write(file, end, strlen(end));
    int pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (!pid) {
        execlp("gcc", "gcc", path, "-o", exec_path, NULL);
        printf("error1\n");
        _exit(1);
    }
    waitpid(pid, NULL, 0);
    int num;
    while (scanf("%d", &num) > 0) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%d", num);
        int pid = fork();
        if (pid < 0) {
            return 1;
        }
        if (!pid) {
            char *argvv[2];
            argvv[0] = exec_path;
            argvv[1] = buf;
            execve(exec_path, argvv, environ);
            printf("error2\n");
            _exit(1);
        }
        waitpid(pid, NULL, 0);
    }
    return 0;
}
