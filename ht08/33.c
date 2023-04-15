#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

enum { BASIS = 10 };

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (file < 0) {
        return 1;
    }
    int rows = strtol(argv[2], NULL, BASIS), cols = strtol(argv[3], NULL, BASIS);
    int size;
    if (__builtin_mul_overflow(rows, cols, &size)) {
        return 1;
    }
    if (__builtin_mul_overflow(size, sizeof(size), &size)) {
        return 1;
    }
    if (ftruncate(file, size) < 0) {
        return 1;
    }
    int *map = (int *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (map == NULL) {
        return 1;
    }
    if (close(file) < 0) {
        return 1;
    }

    int *been = (int *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    int dr[4] = { 0, 1, 0, -1 };
    int dc[4] = { 1, 0, -1, 0 };
    int row = 0, col = 0, di = 0, cnt = 0;
    for (int i = 0; i < rows * cols; i++) {
        map[row * cols + col] = ++cnt;
        been[row * cols + col] = 1;
        int new_r = row + dr[di];
        int new_c = col + dc[di];
        if (0 <= new_r && new_r < rows && 0 <= new_c && new_c < cols 
                && !been[new_r * cols + new_c]) {
            row = new_r;
            col = new_c;
        } else {
            di = (di + 1) % 4;
            row += dr[di];
            col += dc[di];
        }
    }

    if (munmap(been, size) < 0) {
        return 1;
    }
    if (munmap(map, size) < 0) {
        return 1;
    }
}
