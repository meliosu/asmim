#define _GNU_SOURCE

#include <immintrin.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE (128 * 1000 * 1000)
#define GB (1000 * 1000 * 1000)

typedef struct {
    pthread_barrier_t *barrier;
    void (*operation)(void *, long);
    int cpu;
} ctx_t;

void memory_read(void *data, long len);
void memory_write(void *data, long len);
void memory_copy(void *data, long len);

void *alloc(int len) {
    return malloc(len);
}

void dealloc(void *data) {
    free(data);
}

void bind_thread(int cpu) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);

    int err = sched_setaffinity(0, sizeof(set), &set);
    if (err) {
        perror("sched_setaffinity");
        exit(1);
    }
}

void *thread(void *arg) {
    ctx_t *ctx = arg;

    bind_thread(ctx->cpu);

    void *data = alloc(SIZE);

    struct timespec beg, end;

    pthread_barrier_wait(ctx->barrier);
    (ctx->operation)(data, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &beg);
    (ctx->operation)(data, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);
    (ctx->operation)(data, SIZE);

    double elapsed = (double)(end.tv_sec - beg.tv_sec) +
                     (double)(end.tv_nsec - beg.tv_nsec) / 1e9;

    double throughput = ((double)SIZE / (double)GB) / elapsed;

    dealloc(data);
    free(ctx);

    return (void *)(long)throughput;
}

int main() {
    int cpus = 6;

    for (int op = 0; op < 3; op++) {
        void (*operation)(void *, long);
        char *name;

        switch (op) {
        case 0:
            operation = memory_read;
            name = "Read";
            break;
        case 1:
            operation = memory_write;
            name = "Write";
            break;
        case 2:
            operation = memory_copy;
            name = "Copy";
            break;
        }

        printf("%s:\n", name);

        for (int n = 1; n <= cpus; n++) {
            printf("%d Threads:\n", n);

            pthread_t tids[n];
            void *times[n];

            pthread_barrier_t barrier;
            pthread_barrier_init(&barrier, NULL, n);

            for (int i = 0; i < n; i++) {
                ctx_t *ctx = malloc(sizeof(*ctx));

                ctx->barrier = &barrier;
                ctx->operation = operation;
                ctx->cpu = i;

                pthread_create(&tids[i], NULL, thread, ctx);
            }

            for (int i = 0; i < n; i++) {
                pthread_join(tids[i], &times[i]);
            }

            for (int i = 0; i < n; i++) {
                printf("%d: %.2lf GB/s\n", i, (double)(long)times[i]);
            }

            pthread_barrier_destroy(&barrier);
        }
    }
}
