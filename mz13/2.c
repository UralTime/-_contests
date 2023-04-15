#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

int64_t 
calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count)
{
    bool ovflow1 = false, ovflow2 = false;
    int64_t block_num = block_size / block_num_size;
    int64_t tmp = 0, res = 0;
    ovflow1 |= __builtin_smulll_overflow(block_num, block_num, &tmp);
    ovflow1 |= __builtin_saddll_overflow(block_num, tmp, (long long*)&res);
    ovflow1 |= __builtin_smulll_overflow(block_num, tmp, &tmp);
    ovflow1 |= __builtin_saddll_overflow(tmp, res, (long long*)&res);
    ovflow1 |= __builtin_saddll_overflow(res, inode_direct_block_count, (long long *)&res);
    ovflow1 |= __builtin_smulll_overflow(res, block_size, &res);
    ovflow2 |= __builtin_smulll_overflow(block_num_size, sizeof(int64_t), &tmp);
    int64_t new_res = 1;
    if (tmp >= sizeof(res) * CHAR_BIT) {
        ovflow2 = 1;
    } else {
        new_res = new_res << tmp;
    }
    ovflow2 |= __builtin_smulll_overflow(block_size, new_res, &new_res);
    if (ovflow1 && ovflow2) {
        return -1;
    } else if (ovflow2) {
        return res;
    } else if (ovflow1) {
        return new_res;
    }
    return ((res < new_res)? res: new_res);
}

int 
main(void)
{
    int64_t bl_sz, bl_num_sz, inode_dir_bl_cnt;
    scanf("%lld %lld %lld", &bl_sz, &bl_num_sz, &inode_dir_bl_cnt);
    int64_t max = calc_max_size(bl_sz, bl_num_sz, inode_dir_bl_cnt);
    printf("%lld\n", max);
    int64_t arg_max = 1;
    max--;
    for (int64_t i = 1; i <= sizeof(max); i++) {
        int64_t cur_calc = calc_max_size(bl_sz, i, inode_dir_bl_cnt);
        if (cur_calc == -1) {
            printf("%lld %lld\n", i, cur_calc);
            return 0;
        }
        if (cur_calc > max) {
            arg_max = i;
            max = cur_calc;
        }
    }
    printf("%lld %lld\n", arg_max, max);
    return 0;
}
