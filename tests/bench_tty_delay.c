#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include "cfg.h"

cfg_t cfg;

void tty_delay(int usec);

int main() {
    struct rusage usage_start, usage_end;
    struct timeval tv_start, tv_end;

    gettimeofday(&tv_start, NULL);
    getrusage(RUSAGE_SELF, &usage_start);

    // Run 100 delays of 10,000 microseconds (10 ms each) = total 1 second delay
    int iterations = 100;
    int delay_usec = 10000;

    for (int i = 0; i < iterations; i++) {
        tty_delay(delay_usec);
    }

    getrusage(RUSAGE_SELF, &usage_end);
    gettimeofday(&tv_end, NULL);

    double wall_time = (tv_end.tv_sec - tv_start.tv_sec) +
                       (tv_end.tv_usec - tv_start.tv_usec) / 1000000.0;

    double user_cpu = (usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec) +
                      (usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec) / 1000000.0;

    double sys_cpu = (usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec) +
                     (usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec) / 1000000.0;

    double total_cpu = user_cpu + sys_cpu;

    printf("Iterations: %d x %d us\n", iterations, delay_usec);
    printf("Wall time:  %.6f seconds\n", wall_time);
    printf("User CPU:   %.6f seconds\n", user_cpu);
    printf("Sys CPU:    %.6f seconds\n", sys_cpu);
    printf("Total CPU:  %.6f seconds\n", total_cpu);
    printf("CPU Usage:  %.2f%%\n", (total_cpu / wall_time) * 100.0);

    return 0;
}
