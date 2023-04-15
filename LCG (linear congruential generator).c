#include <stdio.h>
#include <stdlib.h>
 
enum
{
    MUL = 1103515245,
    INC = 12345,
    MOD = 1u << 31
};
 
typedef struct RandomGenerator
{
    unsigned mul, inc, mod;
    unsigned elem;
    const struct RandomOperations *ops;
} RandomGenerator;
 
static int next(RandomGenerator *rr)
{
    if (rr == NULL) {
        return 1;
    }
    unsigned res = 0;
    if (__builtin_mul_overflow(rr->mul % rr-> mod, rr->elem % rr->mod, &res)) {
        fprintf(stderr, "overflow\n");
        return 1;
    }
    if (__builtin_add_overflow(res % rr->mod, rr->inc % rr->mod, &res)) {
        fprintf(stderr, "overflow\n");
        return 1;
    }
    rr->elem = res % rr->mod;
    return rr->elem;
}
 
static void destroy(RandomGenerator *rr)
{
    if (rr == NULL) {
        return;
    }
    free(rr);
}
 
typedef const struct RandomOperations
{
    void (*destroy) (RandomGenerator *rr);
    int (*next) (RandomGenerator *rr);
} RandomOperations;
 
static RandomOperations DefaultRandomOperations = {destroy, next};
 
RandomGenerator *random_create(int seed)
{
    RandomGenerator *generator = calloc(1, sizeof(*generator));
    if (generator == NULL) {
        return NULL;
    }
    generator->ops = &DefaultRandomOperations;
    generator->mul = MUL;
    generator->mod = MOD;
    generator->inc = INC;
    generator->elem = seed;
    return generator;
}
