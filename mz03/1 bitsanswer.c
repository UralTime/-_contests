#include <limits.h>
STYPE bit_reverse(STYPE value)
{
    UTYPE uns_value = (UTYPE) value, test_value = 0;
    int bits = sizeof(value) * CHAR_BIT;
    for (int i = 0; i < bits - 1; i++) {
        test_value += uns_value & 1;
        uns_value >>= 1;
        test_value <<= 1;
    }
    test_value += uns_value & 1;
    return (STYPE) test_value; 
}
