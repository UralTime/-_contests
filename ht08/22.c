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

typedef const struct RandomOperations
{
    void (*destroy) (RandomGenerator *rr);
    int (*next) (RandomGenerator *rr);
} RandomOperations;

static int next(RandomGenerator *rr)
{   
    rr->elem = (rr->mul * rr->elem + rr->inc) % rr->mod;
    return rr->elem;
}

static void destroy(RandomGenerator *rr)
{
    if (rr == NULL) {
        return;
    }
    free(rr);
}

RandomGenerator *random_create(int seed)
{
    RandomGenerator *generator = calloc(1, sizeof(*generator));
    if (generator == NULL) {
        return NULL;
    }
    static const RandomOperations ROPS = {destroy, next};
    generator->ops = &ROPS;
    generator->mul = MUL;
    generator->mod = MOD;
    generator->inc = INC;
    generator->elem = seed;
    return generator;
}
