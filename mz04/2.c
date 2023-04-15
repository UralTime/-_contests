#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
DFS(int pos, int file)
{
    struct Node cur_node;
    if (lseek(file, pos * sizeof(struct Node), SEEK_SET) == -1)
        return;
    if (read(file, &cur_node, sizeof(struct Node)) != sizeof(struct Node))
        return;
    if (cur_node.right_idx > 0)
        DFS(cur_node.right_idx, file);
    printf("%d\n", cur_node.key);
    if (cur_node.left_idx > 0)
        DFS(cur_node.left_idx, file);
}

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_RDONLY, 0777);
    if (file == -1)
        return 1;
    DFS(0, file);
    close(file);
    return 0;
}
