#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        if (argv[2][0] == '/') {
            argv[2][0] = '-';
        }
        long int x;
        char p = '\0';
        int m = sscanf(argv[1], "%ld%c", &x, &p);
        if (!m || p!='\0') {
            fprintf(stderr, "Error: The number was not entered\n");
            return 1;
        }
        else if (strcmp(argv[2], "-h") == 0) {
            if (x == 0) {
                printf("No multiples of %ld found\n", x);
                return 0;
            }
            bool found = false;
            for (int i = 1; i <= 100; i++) {
                if (i % x == 0) {
                    printf("%d\n", i);
                    found = true;
                }
            }
            if (!found) {
                printf("No multiples of %ld found\n", x);
            }
        }
        else if (strcmp(argv[2], "-p") == 0) {
            bool is_prime = true;
            if (x < 2) {
                printf("%ld is neither prime nor composite.\n", x);
            }
            else{
                for (int i = 2; i < x; i++) {
                    if (x % i == 0) {
                        is_prime = false;
                        break;
                    }
                }
                if (is_prime) {
                    printf("%ld is prime.\n", x);
                }
                else {
                    printf("%ld is composite.\n", x);
                }
            }
        }
        else if (strcmp(argv[2], "-e") == 0) {
            if (x > 10) {
                fprintf(stderr, "Error: %ld is too big for this function\n", x);
                return 1;
            }
            if (x < 1) {
                fprintf(stderr, "Error: %ld is too small\n", x);
                return 1;
            }
            else {
                for (int i = 1; i <= 10; i++) {
                    long long res = i;
                    for (int m = 1; m <= x; m++) {
                        printf("%2d^%d=%-9lld ", i, m, res);
                        res = res * i;
                    }
                    printf("\n");
                }
            }
        }
        else if (strcmp(argv[2], "-a") == 0) {
            long int res = 0;
	    int is_work = 0;
            for (int i = 1; i <= x; i++) {
                if (res > LONG_MAX - i) {
                    fprintf(stderr, "Error: %ld is too big for this function\n", x);
		    is_work ++;
                    break;
                }
                res += i;
            }
	    if(is_work == 0){
           	 printf("%ld\n", res);
	    }
        }
        else if (strcmp(argv[2], "-f") == 0) {
            if (x < 0) {
                fprintf(stderr, "Error: factorial of %ld is undefined\n", x);
                return 1;
            }
            if (x > 20) {
                fprintf(stderr, "Error: %ld is too big for this function\n", x);
                return 1;
            }
            long int fact = 1;
            for (long int i = x; i >= 1; i--) {
                fact *= i;
            }
            printf("%ld\n", fact);
        }
        else if (strcmp(argv[2], "-s") == 0) {
            if (x < 0) {
                fprintf(stderr, "Error: negative number\n");
                return 1;
            }
            if (x == 0) {
                printf("%ld", x);
            }
            char hexx[17];
            int n = 0;
            while (x > 0) {
                int digit = x % 16;
                if (digit >= 10) {
                    hexx[n] = 'A' + (digit - 10);
                }
                else {
                    hexx[n] = '0' + digit;
                }
                x = x / 16;
                n++;
            }
            for (int i = n - 1; i >= 0; i--) {
                printf("%c ", hexx[i]);
            }
        }
        else {
            fprintf(stderr, "Error: unknown flag '%s'\n", argv[2]);
            return 1;
        }
    }
    else {
        printf("Usage: %s <number> <flag>\n", argv[0]);
        return 1;
    }
}
