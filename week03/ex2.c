#include <stdio.h>
#include <math.h>

struct Point
{
    double x;
    double y;
};

double distance(struct Point A, struct Point B)
{
    return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

double area(struct Point A, struct Point B, struct Point C)
{
    return 0.5 * fabs(A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y);
}

int main()
{
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};

    double distAB = distance(A, B);

    double triangleArea = area(A, B, C);

    printf("Distance between A and B: %.2lf\n", distAB);
    printf("Area of triangle ABC: %.2lf\n", triangleArea);

    return 0;
}

