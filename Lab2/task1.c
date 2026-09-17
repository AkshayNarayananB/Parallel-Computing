#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int N = 100000000; // Input size N=100000000
    
    // Allocate on heap to avoid stack overflow
    int *arr = (int*)malloc(N * sizeof(int));
    if (arr == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    // Each element is assigned from 0 to N-1
    for (int i = 0; i < N; i++) {
        arr[i] = i;
    }

    long long sum = 0;
    double start = omp_get_wtime();
    
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    
    double end = omp_get_wtime();

    printf("Sum: %lld\n", sum);
    printf("Time: %f seconds\n", end - start);
    
    free(arr);
    return 0;
}
