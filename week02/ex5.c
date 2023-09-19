#include <stdio.h>

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int t0 = 0, t1 = 1, t2 = 1, tn;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }

    return tn;
}

int main() {
    int result1 = tribonacci(4);
    int result2 = tribonacci(36);

    printf("Tribonacci(4) = %d\n", result1);
    printf("Tribonacci(36) = %d\n", result2);

    return 0;
}
