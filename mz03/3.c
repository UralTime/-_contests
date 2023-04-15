#include <stdio.h>
#include <math.h>
#include <stdlib.h>

enum 
{
    FULL_PERCENTS = 100, // 1% from 1 is 0.01, so we should divide by 100 after multiply pe percents
    POS_START_CHANGES = 2, // changes starts from second argument of command line
    ROUNDING_K = 10000 // task condition: The exchange rate is fixed with four digits of the fractional part 
};

int main(int argc, char *argv[])
{
    char *err;
    double cur_rate = strtod(argv[1], &err);
    for (int i = POS_START_CHANGES; i < argc; ++i) {
        double percents = strtod(argv[i], &err);
        double change_k = 1.0 + percents / FULL_PERCENTS;
        cur_rate *= change_k;
        cur_rate = round(cur_rate * ROUNDING_K) / ROUNDING_K;
    };
    printf("%.4f\n", cur_rate); // task condition: Type the rate with four decimal places.
    return 0;
}
