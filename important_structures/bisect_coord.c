#include <stdio.h>
#include <math.h>

struct point {
    float x, y;
};

struct point bisect(struct point p1, struct point p2, struct point p3) {
    float p1p2 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    float p2p3 = (p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y);
    float k = sqrt(p1p2 / p2p3);
    struct point p;
    p.x = (p1.x + k * p3.x) / (k + 1);
    p.y = (p1.y + k * p3.y) / (k + 1);
    return p;
}

int main(void) {
    struct point A, B, C;
    scanf("%f%f%f%f%f%f", &(A.x), &(A.y), &(B.x), &(B.y), &(C.x), &(C.y));
    struct point A1 = bisect(B, A, C), B1 = bisect(A, B, C), C1 = bisect(A, C, B);
    printf("%f %f \n%f %f \n%f %f", C1.x, C1.y, A1.x, A1.y, B1.x, B1.y);
    return 0;
}
