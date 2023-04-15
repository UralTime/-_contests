int main() {


    pid_t pid = fork();

    if (pid == 0) {

         close(0);
         int fd = open("2.c", O_RDONLY);

         execlp("cat", "cat", NULL);
         perror("exec");
         _exit(0);
    }

    wait(NULL);
}

