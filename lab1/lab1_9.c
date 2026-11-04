#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIZE 20

void fill_random(int * digits, int a, int b, int size){
    for(int i = 0; i < size; i++){
        int r = rand() % (b - a + 1) + a;
        digits[i] = r;
    }
}

void find_min_max(int * digits, int size, int * mini, int* maxi){
    *mini = *maxi = 0;
    for(int i = 1; i < size; i++){
        if(digits[i] < digits[*mini]) *mini = i;
        if(digits[i] > digits[*maxi]) *maxi = i;
    }
}

void swap(int mini, int maxi, int* digits){
    int cur = digits[mini];
    digits[mini] = digits[maxi];
    digits[maxi] = cur;
}

void print_arr(int *digits, int size){
    int limit;
    if(size <= SIZE) limit = size;
    else limit = SIZE;

    for(int i = 0; i < limit; i++) printf("%d ", digits[i]);

    if(size > SIZE) printf("...(total %d)", size);
    printf("\n");
}

int find_closest(int*B, int x, int size){
    int closest = 0;
    for(int i = 1; i < size; i++){
        if(abs(x - B[i]) < abs(x - B[closest])) closest = i;
    }
    return B[closest];
}

void make_c(int * A, int* B, int * C, int size_a, int size_b){
    for(int i = 0; i < size_a; i++){
        C[i] = A[i] + find_closest(B, A[i], size_b);
    }
}

int main(int argc, char * argv[]){
    srand(time(NULL));
    if(argc != 3){
        fprintf(stderr, "USAGE: %s <a> <b>\n", argv[0]);
        return 1;
    }
    int a;
    int b;
    int m = sscanf(argv[1], "%d", &a);
    m += sscanf(argv[2], "%d", &b);
    if(m!= 2){
        fprintf(stderr, "USAGE: a, b must be int.\n");
        return 1;
    }
    if(a > b){
        fprintf(stderr, "a must be <= b\n");
        return 1;
    }
    int digits[SIZE];
    fill_random(digits, a, b, SIZE);
    printf("Task 1. before:\n");
    print_arr(digits, SIZE);
    int mini, maxi;
    find_min_max(digits, SIZE, &mini, &maxi);
    swap(mini, maxi, digits);
    printf("after:\n");
    print_arr(digits, SIZE);

    int rand_size_a = rand() % (10000 - 10 + 1) + 10;
    int rand_size_b = rand() % (10000 - 10 + 1) + 10;
    int* A = malloc(rand_size_a * sizeof(int));
    int* B = malloc(rand_size_b * sizeof(int));
    int* C = malloc(rand_size_a * sizeof(int));
    if(!A || !B || !C){
        free(A); free(B); free(C);
        return 1;
    }
    fill_random(A, -1000, 1000, rand_size_a);
    fill_random(B, -1000, 1000, rand_size_b);

    make_c(A, B, C, rand_size_a, rand_size_b);

    printf("Task 2.\n");
    printf("A:\n");
    print_arr(A, rand_size_a);
    printf("B:\n");
    print_arr(B, rand_size_b);
    printf("C:\n");
    print_arr(C, rand_size_a);
    free(A);
    free(B);
    free(C);
    return 0;
}
