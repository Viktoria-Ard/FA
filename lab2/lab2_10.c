#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int func(double eps, double a, double ** res, int step, ...){
    va_list arg;
    va_start(arg, step);

    double *polyn = (double*)malloc((step + 1) * sizeof(double));
    if(!polyn){
        *res = NULL;
        return -1;
    }

    for(int i = 0; i <= step; i++){
        polyn[i] = va_arg(arg, double);
    }
    va_end(arg);

    double *result = (double*)malloc((step+1) * sizeof(double));
    double *curr = (double*)malloc((step + 1) * sizeof(double)); //перевернутый (от старшего к младшему)
    double *quotient = (double*)malloc((step + 1) * sizeof(double));

    if(!result || !curr || !quotient){
        free(polyn);
        free(result);
        free(curr);
        free(quotient);
        return -1;
    }

    for(int i = 0; i <= step; i++) result[i] = 0.0;

    for(int i = 0; i <= step; i++){
        curr[i] = polyn[step - i];
    }

    int len = step + 1;

    for(int k = 0; k <= step; k++){
        quotient[0] = curr[0];
        for(int i = 1; i < len; i++){
            quotient[i] = a * quotient[i-1] + curr[i];
        }

        double g = quotient[len - 1];

        for(int i = 0; i < len - 1; i++){
            curr[i] = quotient[i];
        }
        len--;

        result[k] = g;

        int is_end = 1;
        for(int i = 0; i < len; i++){
            if(fabs(curr[i]) >= eps){
                is_end = 0;
                break;
            }
        }
        if(is_end == 1) break;
    }
    free(polyn);
    free(curr);
    free(quotient);
    *res = result;
    return 0;
}

int main(){
    double *g = NULL;

    // f(x) = x^3 - 5x^2 + 8, a = 2 
    func(1e-12, 2.0, &g, 3, 8.0, 0.0, -5.0, 1.0); //от младшего к старшего

    printf("g = [");
    for (int k = 0; k <= 3; ++k)
        printf("%s%g", k ? ", " : "", g[k]);
    printf("]\n");

    free(g);
    return 0;
}