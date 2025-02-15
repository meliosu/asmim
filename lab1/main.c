#include <immintrin.h>
#include <stdio.h>
#include <x86intrin.h>

#define OPERATIONS 60000000

typedef unsigned long long u64;

static void used(__m128d value) {
    asm volatile("" : "+xmm"(value));
}

void warmup() {
    __m128d value = _mm_set1_pd(2.0);

    for (int i = 0; i < OPERATIONS; i++) {
        value = _mm_mul_pd(value, value);
    }

    used(value);
}

double measure_latency() {
    __m128d value = _mm_set1_pd(2.0);

    u64 start = __builtin_ia32_rdtsc();

    for (int i = 0; i < OPERATIONS / 4; i++) {
        value = _mm_mul_pd(value, value);
        value = _mm_mul_pd(value, value);
        value = _mm_mul_pd(value, value);
        value = _mm_mul_pd(value, value);
    }

    u64 end = __builtin_ia32_rdtsc();

    used(value);

    return (double)(end - start) / (double)OPERATIONS;
}

double measure_throughput() {
    __m128d value0 = _mm_set1_pd(2.0);
    __m128d value1 = _mm_set1_pd(3.0);
    __m128d value2 = _mm_set1_pd(4.0);
    __m128d value3 = _mm_set1_pd(5.0);
    __m128d value4 = _mm_set1_pd(6.0);

    u64 start = __builtin_ia32_rdtsc();

    for (int i = 0; i < OPERATIONS / 5; i++) {
        value0 = _mm_mul_pd(value0, value0);
        value1 = _mm_mul_pd(value1, value1);
        value2 = _mm_mul_pd(value2, value2);
        value3 = _mm_mul_pd(value3, value3);
        value4 = _mm_mul_pd(value4, value4);
    }

    u64 end = __builtin_ia32_rdtsc();

    used(value0);
    used(value1);
    used(value2);
    used(value3);
    used(value4);

    return (double)OPERATIONS / (double)(end - start);
}

int main() {
    warmup();

    double latency = measure_latency();
    double throughput = measure_throughput();

    printf(
        "Latency: %.3lf\n"
        "Throughput: %.3lf\n",
        latency,
        throughput
    );
}
