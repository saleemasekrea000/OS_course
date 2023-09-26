#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include<string.h>

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*, size_t))
{
    void* result = malloc(size);
    memcpy(result, initial_value, size);
    for (int i = 0; i < n; i++)
    {
        void* current_element = (char*)base + i * size;
        result = opr(result, current_element, size);
    }
    return result;
}

void* addition(const void* a, const void* b, size_t size)
{
    if (size == sizeof(double))
    {
        double* result = malloc(sizeof(double));
        *result = *((double*)a) + *((double*)b);
        return result;
    }
    else if (size == sizeof(int))
    {
        int* result = malloc(sizeof(int));
        *result = *((int*)a) + *((int*)b);
        return result;
    }
}

void* multiplication(const void* a, const void* b, size_t size)
{
    if (size == sizeof(double))
    {
        double* result = malloc(sizeof(double));
        *result = *((double*)a) * *((double*)b);
        return result;
    }
    else if (size == sizeof(int))
    {
        int* result = malloc(sizeof(int));
        *result = *((int*)a) * *((int*)b);
        return result;
    }
}

void* max(const void* a, const void* b, size_t size)
{
    if (size == sizeof(double))
    {
        if (*((double*)a) > *((double*)b))
        {
            return (void*)a;
        }
        else
        {
            return (void*)b;
        }
    }
    else if (size == sizeof(int))
    {
        if (*((int*)a) > *((int*)b))
        {
            return (void*)a;
        }
        else
        {
            return (void*)b;
        }
    }
    else
    {
        return NULL;
    }
}

int main()
{
    double double_array[] = {1.1, 2.2, 3.3, 4.8, 5.5};
    double initial_double = 0.0;
    double* result_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double, addition);
    printf("Sum of doubles: %.2f\n", *result_double);

    double initial_double_product = 1.0;
    double* result_double_product = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_product, multiplication);
    printf("Product of doubles: %.2f\n", *result_double_product);

    int int_array[] = {1, 2, 3, 4, 5};
    int initial_int = 1;
    int* result_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int, multiplication);
    printf("Product of integers: %d\n", *result_int);

    int initial_int_sum = 0;
    int* result_int_sum = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_sum, addition);
    printf("Sum of integers: %d\n", *result_int_sum);


    int initial_max = INT_MIN;
    int* result_max = (int*)aggregate(int_array, sizeof(int), 5, &initial_max, max);
    printf("Max of integers: %d\n", *result_max);

    double initial_max_double = -DBL_MAX;
    double* result_max_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_max_double, max);
    printf("Max of doubles: %.2f\n", *result_max_double);
    return 0;
}
