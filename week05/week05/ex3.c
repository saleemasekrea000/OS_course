#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}
int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++)
        if (is_prime(i))
            ret++;
    return ret;
}
typedef struct prime_request {
    int a, b;
} prime_request;
void *prime_counter(void *arg) {
    prime_request *req = (prime_request *)arg;
    int count = primes_count(req->a, req->b);
    int *result = (int *)malloc(sizeof(int));
    if (result == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    *result = count;
    return (void *)result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    pthread_t threads[m];
    prime_request thread_args[m];
    int interval_size = n / m;
    int remainder = n % m;

    for (int i = 0; i < m; i++) {
        thread_args[i].a = i * interval_size;
        thread_args[i].b = (i == m - 1) ? (i + 1) * interval_size + remainder : (i + 1) * interval_size;

        pthread_create(&threads[i], NULL, prime_counter, &thread_args[i]);
    }
    int total_count = 0;
    for (int i = 0; i < m; i++) {
        int *count;
        pthread_join(threads[i], (void **)&count);
        total_count += *count;
        free(count);
    }
    printf("Total primes in [0, %d) = %d\n", n, total_count);
    return 0;
}

