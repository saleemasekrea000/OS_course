#include <stdio.h>
#include <stdlib.h>

void print_memory_addresses(const int *p, int n) {
    printf("Memory Addresses:\n");
    for (int i = 0; i < n; i++) {
        printf("Cell %d: %p\n", i + 1, p + i);
    }
}

int const_tri(int *p, int n) {
    if (n <= 0) return 0;
    if (n <= 3) return 1;

    for (int i = 3; i < n; i++) {
        int temp = p[i - 1] + p[i - 2] + p[i - 3];
        p[i] = temp;
    }

    return p[n - 1];
}

int main() {
    const int x = 1;
    const int *q = &x;
    int *p = (int *)malloc(3 * sizeof(int));

    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    p[0] = x;
    p[1] = x;
    p[2] = 2 * x;
    print_memory_addresses(p, 3);

    int n = 8;
    int result = const_tri(p, n);
    printf("Tribonacci(%d) = %d\n", n, result);

    free(p);

    return 0;
}
