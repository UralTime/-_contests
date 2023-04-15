#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(void)
{
    unsigned short cur_vote[255] = {0}, votes[7] = {0}, cur_votes[7] = {0};
    unsigned long long sum = 0;
    int read_cnt;
    while ((read_cnt = read(0, cur_vote, 255)) > 0) {
        int diff = cur_vote[1] - sum;
        if (diff) {
            votes[6] += cur_votes[6];
            for (int i = 6; i > 2; i--) {
                votes[i] += cur_votes[i - 1];
                diff -= cur_votes[i - 1];
            }
            votes[2] += diff;
            votes[1] += cur_votes[1] - diff;
        }
        else
            for (int i = 6; i > 0; i--)
                votes[i] += cur_votes[i];
        for (unsigned short i = 0; i < cur_vote[0]; i++) {
            sum += cur_vote[i + 2];
            cur_votes[cur_vote[i + 2]]++;
        }
    };
    for (int i = 1; i <= 6; i++)
        printf("%d", votes[i]);
    return 0;
}
