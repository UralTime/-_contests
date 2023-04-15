#include <sys/types.h>
#include <sys/stat.h>

enum 
{
    OWNER_OFFSET = 6,
    GROUP_OFFSET = 3
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static int 
check(int rights, int access) 
{
    return ((rights & (unsigned) access) == (unsigned) access);
}

int 
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (!task || !stb) {
        return 0;
    }
    if (!task->uid) {
        return 1;
    }
    if (stb->st_uid == task->uid) {
        return check(stb->st_mode >> OWNER_OFFSET, access);
    }
    if (!task->gid_count || !task->gids) {
        return 0;
    }
    for (int i = 0; i < task->gid_count; ++i) {
        if (stb->st_gid == task->gids[i]) {
            return check(stb->st_mode >> GROUP_OFFSET, access);
        }
    }
    return check(stb->st_mode, access);
}
