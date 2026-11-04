#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define EPS 1e-9

typedef struct
{
    double *coords;
}Vector;

double *get_coords(Vector *v){
    return v->coords;
}

typedef struct 
{
    int count;
    int *inds;
    double max_len;
}Norm_result;

typedef double (*NormFunc)(Vector *v, int n, void *params);

typedef struct 
{
    int n;
    double ** data;
}Matrix;

Matrix *create_matr(int n){
    Matrix *A = malloc(sizeof(Matrix));
    A->n = n;
    A->data = (double**)malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++) A->data[i] = (double*)malloc(n * sizeof(double));
    return A;
}

void free_matr(Matrix *A){
    for(int i = 0; i < A->n; i++) free(A->data[i]);
    free(A->data);
    free(A);
}

double norm_inf(Vector *v, int n, void *params){
    double max = 0.0;
    double *coords = get_coords(v);
    for(int i = 0; i < n; i++){
        if(fabs(coords[i]) > max) max = fabs(coords[i]);
    }
    return max;
}

double norm_p(Vector *v, int n, void *params){
    if (!params) return -1.0;
    double p = *(double*)(params);
    double sum = 0.0;
    double *coords = get_coords(v);
    for(int i = 0; i < n; i++){
        sum += pow(fabs(coords[i]), p);
    }
    return pow(sum, 1.0/p);
}

double norm_a(Vector *v, int n, void *params){
    if(!params) return -1;
    Matrix *A = (Matrix*)(params);
    double ** data = A->data;
    double Ax[n];
    for(int i = 0; i < n; i++) Ax[i] = 0.0;
    double *coords = get_coords(v);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            Ax[i] += data[i][j] * coords[j];
        }
    }
    double res = 0.0;
    for(int i = 0; i < n; i++){
        res += Ax[i] * coords[i];
    }
    return sqrt(res);
}

void find_longest(int n, int cnt_norms, int cnt_vectors, Norm_result *results, ...){
    va_list arg;
    va_start(arg, results);

    NormFunc *norms = malloc(cnt_norms * (sizeof(NormFunc)));
    void ** params = malloc(cnt_norms * sizeof(void*));

    for(int i = 0; i < cnt_norms; i++){
        norms[i] = va_arg(arg, NormFunc);
        params[i] = va_arg(arg, void*);
    }

    Vector *vectors = malloc(cnt_vectors * sizeof(Vector));
    for(int i = 0; i < cnt_vectors; i++){
        vectors[i] = va_arg(arg, Vector);
    }
    va_end(arg);

    for(int i = 0; i < cnt_norms; i++){
        double maxi = -1.0;

        for(int v = 0; v < cnt_vectors; v++){ //поиск максимальной длины
            double len = norms[i](&vectors[v], n, params[i]);
            if(len > maxi) maxi = len;
        }
        results[i].max_len = maxi;

        int count_maxi = 0;
        for(int v = 0; v < cnt_vectors; v++){ //подсчет количества самых длинных
            double len = norms[i](&vectors[v], n, params[i]);
            if(fabs(len - maxi) < EPS) count_maxi++;
        }

        results[i].count = count_maxi;
        results[i].inds = malloc(count_maxi * sizeof(int));

        int idx = 0;
        for(int v = 0; v< cnt_vectors; v++){ //заполнение массива индексов
            double len = norms[i](&vectors[v], n, params[i]);
            if(fabs(len - maxi) < EPS) results[i].inds[idx++] = v;
        }
    }
    free(norms);
    free(params);
    free(vectors);
}

int main(){
    int n = 3;
    int cnt_norms = 3;
    int cnt_vectors = 4;

    double c0[] = {3.0, 4.0, 0.0};
    double c1[] = {1.0, 2.0, 2.0};
    double c2[] = {-3.0, 0.0, 4.0};
    double c3[] = {0.0, 0.0, 1.0};

    Vector v0 = {c0};
    Vector v1 = {c1};
    Vector v2 = {c2};
    Vector v3 = {c3};

    double p = 2.0;

    Matrix *A = create_matr(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            A->data[i][j] = 0.0;
        }
    }
    A->data[0][0] = 4.0;
    A->data[1][1] = 1.0;
    A->data[2][2] = 1.0;

    Norm_result results[3];
    find_longest(n, cnt_norms, cnt_vectors, results, 
        norm_inf, NULL,
        norm_p, &p, 
        norm_a, A, 
        v0, v1, v2, v3);

    char *names[] = {"inf", "p=2", "A(diag 4, 1, 1)"};
    for(int i = 0; i < cnt_norms; i++){
        printf("НОРМА %s:\n", names[i]);
        printf("Мaксимальная длина: %f\n", results[i].max_len);
        printf("Индексы векторов с максимальной длиной: ");
        for(int j = 0; j < results[i].count; j++){
            printf("%d ", results[i].inds[j]);
        }
        printf("\n");
        free(results[i].inds);
    }
    free_matr(A);
    return 0;
}