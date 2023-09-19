#include <stdio.h>

int main() {
    // Pattern 1
    printf("Pattern 1:\n");
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }
    for (int i = 3; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }

    // Pattern 2
    printf("\nPattern 2:\n");
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }

    // Pattern 3
    printf("\nPattern 3:\n");
    for (int i = 1; i <= 7; i++) {
        for (int j = 1; j <= 7; j++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
