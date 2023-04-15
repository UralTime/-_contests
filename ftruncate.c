#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h> 
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    const char *name = argv[1];
    int count = strtol(argv[2], NULL, 10);

    int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) abort();

    size_t z = count * sizeof(datatype_t);
    ftruncate(fd, z);

    datatype_t *ptr = mmap(NULL, z, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) abort();
    close(fd);

    for (int i = count - 1, j = 1; i >= 0; --i, ++j) {
        ptr[i] = j;
    }

    munmap(ptr, z);

}

