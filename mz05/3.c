#include <inttypes.h>
#include <stdio.h>

enum 
{ 
    CNT_PARAMS = 3,
    BITS_IN_INT32 = 32
};

int
main(void)
{
    uint32_t deg, step;
    int width;
    if (scanf("%"SCNu32"%"SCNu32"%d", &deg, &step, &width) != CNT_PARAMS) {
        return 1;
    }
    uint32_t max = (deg != BITS_IN_INT32)? 1u << deg : UINT32_MAX;
    for (uint32_t cnt = 0; cnt < max; cnt += step) {
        int32_t ans = cnt;
        if (deg == 1) {
            ans = 0;
        } else if (cnt >> (deg - 1)) {
            ans = (-1) * ((cnt << (BITS_IN_INT32 - deg + 1)) >> (BITS_IN_INT32 - deg + 1));
        }
        printf("|%*" PRIo32"|%*" PRIu32"|%*" PRId32"|\n", width, cnt, width, cnt, width, ans);
        if (UINT32_MAX - step < cnt) {
            break;
        }
    }
    return 0;
}
