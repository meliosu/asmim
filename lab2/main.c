#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NUM_NOPS
    #define NUM_NOPS 0
#endif

#define STRINGIFY(X) #X
#define NOP(N)                                                                 \
    asm volatile(".rept " STRINGIFY(N) "\n"                                    \
                                       "nop\n"                                 \
                                       ".endr\n")

#define DATA_SIZE (1000 * 1000 * 16)
#define OPERATIONS (1000 * 1000 * 1024)

void used(long value) {
    asm volatile("" : "+r"(value));
}

void shuffle(long *array, long len) {
    for (long i = len - 1; i > 0; i--) {
        long j = rand() % (i + 1);

        long temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

long *create_cyclic_array(long len) {
    long *array = malloc(len * sizeof(long));

    for (long i = 0; i < len; i++) {
        array[i] = i;
    }

    shuffle(array, len);

    return array;
}

void warmup(long *array) {
    long index = 0;

    for (long i = 0; i < OPERATIONS; i++) {
        index = array[index];
    }

    used(index);
}

uint64_t workload(long *array, long operations) {
    long index = 0;

    uint64_t beg = __builtin_ia32_rdtsc();

    while (operations--) {
        index = array[index];

        NOP(NUM_NOPS);
    }

    uint64_t end = __builtin_ia32_rdtsc();

    used(index);

    return end - beg;
}

int main() {
    long *array = create_cyclic_array(DATA_SIZE / sizeof(long));
    warmup(array);

    uint64_t clocks = workload(array, OPERATIONS);

    printf("%.3lf\n", (double)clocks / (double)OPERATIONS);
    free(array);
}
