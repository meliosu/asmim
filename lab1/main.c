#include <immintrin.h>
#include <stdio.h>
#include <x86intrin.h>

#define OPERATIONS 200000000

typedef unsigned long long u64;

void run_latency(int ops);
void run_throughput(int ops);

int main() {
    u64 start, end;

    start = __rdtsc();
    run_latency(OPERATIONS);
    end = __rdtsc();
    double latency = (double)(end - start) / (double)OPERATIONS;

    start = __rdtsc();
    run_throughput(OPERATIONS);
    end = __rdtsc();
    double throughput = (double)OPERATIONS / (double)(end - start);

    printf(
        "LATENCY: %.2lf\n"
        "THROUGHPUT: %.2lf\n",
        latency,
        throughput
    );
}
