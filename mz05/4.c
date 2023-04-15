#include <string.h>

int 
parse_rwx_permissions(const char *str) 
{
    const char rwx[] = "rwxrwxrwx";
    int len = sizeof(rwx) - 1, ans = 0;
    if (!str || strlen(str) != len) {
        return -1;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] == rwx[i]) {
            ans |= (1 << (len - 1 - i));
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return ans;
}
