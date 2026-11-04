#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define MAX_STEPS 15

int is_integer(double x, double eps) {
    return fabs(x - round(x)) < eps;
}

int is_terminated(int base, double eps, int count, ...) {
    if (base < 2 || base > 36) {
        fprintf(stderr, "Error: base must be in range [2...36]\n");
        return 1;
    }
    if (count <= 0) {
        fprintf(stderr, "Error: count must be > 0.\n");
        return 1;
    }

    va_list arg;
    va_start(arg, count);

    for (int i = 0; i < count; i++) {
        double frac = va_arg(arg, double);

        if (frac <= 0.0 || frac >= 1.0) {
            fprintf(stderr, "Error: Fraction %.10f out of range (0, 1)\n", frac);
            printf("Fraction %.10f: SKIPPED (out of range)\n", frac);
            continue;
        }

        double temp = frac;
        int is_finite = 0;

        for (int step = 0; step < MAX_STEPS; step++) {
            if (is_integer(temp, eps)) {
                is_finite = 1;
                break;
            }
            temp *= base;
        }

        if (is_finite) {
            printf("Fraction %.10f: FINITE in base %d\n", frac, base);
        } else {
            printf("Fraction %.10f: INFINITE in base %d\n", frac, base);
        }
    }

    va_end(arg);
    return 0;
}

int main() {
    printf("Base 10:\n");
    is_terminated(10, 1e-9, 6, 1.0/3.0, 0.5, 0.25, 0.1, 0.333, 1.0/7.0);

    printf("\nBase 2:\n");
    is_terminated(2, 1e-9, 4, 0.5, 0.25, 0.1, 1.0/3.0);

    printf("\nBase 3:\n");
    is_terminated(3, 1e-9, 1, 0.1);

    return 0;
}

