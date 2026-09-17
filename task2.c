#include <stdio.h>
#include <omp.h>
#include <stdbool.h>

#define MAX_THREADS 128

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main() {
    int N = 20000000; // Increased to 20 million for linear scaling
    int total_primes = 0;
    double thread_times[MAX_THREADS] = {0};

    double start_total = omp_get_wtime();
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        double t_start = omp_get_wtime();
        int local_primes = 0;

        #pragma omp for schedule(runtime)
        for (int i = 2; i < N; i++) {
            if (is_prime(i)) {
                local_primes++;
            }
        }

        double t_end = omp_get_wtime();
        thread_times[tid] = t_end - t_start;

        #pragma omp atomic
        total_primes += local_primes;
    }
    
    double end_total = omp_get_wtime();

    // Calculate load balancing efficiency
    int actual_threads = omp_get_max_threads();
    double min_time = thread_times[0];
    double max_time = thread_times[0];
    
    for(int i = 1; i < actual_threads; i++) {
        if(thread_times[i] < min_time) min_time = thread_times[i];
        if(thread_times[i] > max_time) max_time = thread_times[i];
    }

    printf("Primes found: %d\n", total_primes);
    printf("Total Execution Time: %f seconds\n", end_total - start_total);
    if (actual_threads > 1) {
        printf("Fastest thread time: %f s\n", min_time);
        printf("Slowest thread time: %f s\n", max_time);
        printf("Load Imbalance (Idle Time): %f seconds\n", max_time - min_time);
    }
    
    return 0;
}
