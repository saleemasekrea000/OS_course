#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int integer_var;
    unsigned short ushort_var;
    long int longint_var;
    float float_var;
    double double_var;

    integer_var = INT_MAX;
    ushort_var = USHRT_MAX;
    longint_var = LONG_MAX;
    float_var = FLT_MAX;
    double_var = DBL_MAX;

    printf("Size of int: %zu bytes\n", sizeof(integer_var));
    printf("Maximum value of int: %d\n", integer_var);

    printf("Size of unsigned short: %zu bytes\n", sizeof(ushort_var));
    printf("Maximum value of unsigned short: %u\n", ushort_var);

    printf("Size of signed long int: %zu bytes\n", sizeof(longint_var));
    printf("Maximum value of signed long int: %ld\n", longint_var);

    printf("Size of float: %zu bytes\n", sizeof(float_var));
    printf("Maximum value of float: %f\n", float_var);

    printf("Size of double: %zu bytes\n", sizeof(double_var));
    printf("Maximum value of double: %lf\n", double_var);

    return 0;
}
