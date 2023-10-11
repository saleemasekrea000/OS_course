#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_THREADS 100

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
int k = 0;
int c = 0;
int n = 0;

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int get_number_to_check() {
    int ret = k;
    if (k != n)
        k++;
    return ret;
}

void increment_primes() {
    c++;
}

void *check_primes(void *arg) {
    while (1) {
        int current_k;
        pthread_mutex_lock(&global_lock);
        current_k = get_number_to_check();
        pthread_mutex_unlock(&global_lock);

        if (current_k >= n) {
            break;
        }

        if (is_prime(current_k)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);
    int m = atoi(argv[2]);
    if (m > MAX_THREADS) {
        fprintf(stderr, "Maximum number of threads supported: %d\n", MAX_THREADS);
        return 1;
    }
    pthread_t threads[MAX_THREADS];
    // Create and start threads
    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Total primes in [0, %d) = %d\n", n, c);
    return 0;
}

