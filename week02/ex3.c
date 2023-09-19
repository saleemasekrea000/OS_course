#include <stdio.h>
#include <string.h>

void convert(long long int x, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }
    char source_digits[] = "0123456789";
    char target_digits[] = "0123456789";
    char result[100];
    result[0] = '\0';
    long long int decimal_number = 0;
    long long int base = 1;
    while (x > 0) {
        int digit = x % 10;
        if (digit >= s) {
            printf("cannot convert!\n");
            return;
        }
        decimal_number += digit * base;
        base *= s;
        x /= 10;
    }
    while (decimal_number > 0) {
        int digit = decimal_number % t;
        char digit_char = target_digits[digit];
        char temp[2];
        temp[0] = digit_char;
        temp[1] = '\0';
        strcat(result, temp);
        decimal_number /= t;
    }
    char reversed_result[100];
    int len = strlen(result);
    for (int i = 0; i < len; i++) {
        reversed_result[i] = result[len - i - 1];
    }
    reversed_result[len] = '\0';
    printf("%s\n", reversed_result);
}

int main() {
    long long int num;
    int source_base, target_base;
    printf("Enter a non-negative number: ");
    scanf("%lld", &num);
    printf("Enter the source base ");
    scanf("%d", &source_base);
    printf("Enter the target base ");
    scanf("%d", &target_base);
    convert(num, source_base, target_base);
    return 0;
}
