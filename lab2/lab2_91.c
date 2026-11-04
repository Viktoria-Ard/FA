#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define MAX_STEPS 15

long long nod(long long a, long long b) { 
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_terminated(int base, double eps, int count, ...) {
    if (base < 2 || base > 36) {
        fprintf(stderr, "Error: base must be in range\n");
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
            continue;
        }

        long long q = 1;
        double temp = frac;
        int is_finite = 0;
        

        for (int step = 0; step < MAX_STEPS; step++) {
            if (fabs(temp - round(temp)) < eps) {
                is_finite = 1;
                break; 
            }
            temp *= base;
            q *= base;
        }
        
        if (!is_finite) {
            printf("Fraction %.10f: INFINITE in base %d\n", frac, base);
            continue;
        }
        
        long long p = (long long)round(temp);

        // 2. Сокращаем дробь p/q
        long long g = nod(p, q);
        p /= g;
        q /= g; 

        long long temp_q = q;
        for (int d = 2; d <= base; d++) {
            if (base % d == 0) {
                while (temp_q % d == 0) {
                    temp_q /= d;
                }
            }
        }

        if (temp_q == 1) {
            printf("Fraction %.10f: FINITE in base %d\n", frac, base);
        } else {
            printf("Fraction %.10f: INFINITE in base %d\n", frac, base);
        }
    }
    va_end(arg);
    return 0;
}

int main() {
    printf("--- Base 10 ---\n");
    is_terminated(10, 1e-9, 5, 0.5, 0.25, 0.1, 1.0/3.0, 1.0/7.0);

    printf("\n--- Base 2 ---\n");
    is_terminated(2, 1e-9, 4, 0.5, 0.25, 0.1, 1.0/3.0);
    
    printf("\n--- Base 3 ---\n");
    is_terminated(3, 1e-9, 2, 1.0/3.0, 1.0/9.0);

    return 0;
}