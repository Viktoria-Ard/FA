#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
    double x;
    double y;
}Point;

bool is_convex(int n, ...){
    if(n < 3) return 0;
    va_list point;
    Point * v = malloc(n * sizeof(Point)); //массив где будут храниться точки
    if(!v) return 0;
    va_start(point, n);
    int pos = 0;
    int neg = 0;


    for(int i = 0; i < n; i++){
        double x = va_arg(point, double);
        double y = va_arg(point, double);
        Point cur = {x, y};
        v[i] = cur;
    }

    for(int i = 0; i < n; i++){
        Point A = v[i];
        Point B = v[(i + 1) % n];
        Point C = v[(i + 2) % n];

        double v_mult = (B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x); //векторное произведение в двумерном пространстве
        if(v_mult > 0) pos++;
        else if (v_mult < 0) neg++;
        if(pos >0 && neg >0){
            free(v);
            va_end(point);
            return 0;     
        }
    }
    free(v);
    va_end(point);
    return 1;
}


double polyn(double x, int n, ...){
    double ans = 0;
    va_list cur;
    va_start(cur, n);
    for(int i = 0; i <= n; i++){
        ans = ans * x + va_arg(cur, double);
    }
    va_end(cur);
    return ans;
}




int is_alpha(int x) {
    return ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'));
}

int is_digit(int x) {
    return (x >= '0' && x <= '9');
}

long to_des(int osn, char * x){
    long ans = 0;
    int digit;
    int i = 0;
    for(; x[i]; i++){
        if(is_alpha(x[i]) && x[i] < 'a') digit = x[i] - 'A' + 10;
        else if(is_alpha(x[i])) digit = x[i] - 'a' + 10;
        else digit = x[i] - '0';
        ans = ans * osn + digit;
    }
    return ans;
}

char * des_to_system(int osn, long x, char * ans, int size){
    char * pres = ans;
    pres[size - 1] = 0;
    pres += size - 2;
    int d;
    if (x == 0) {
        *pres = '0';
        return pres;
    }
    while(x){
        if((d = x % osn) > 9) *pres = d - 10 + 'A';
        else *pres = d + '0';
        pres --;
        x = x / osn;
    }
    return pres + 1;
}

int validate(char * num, int osn){ //валидация входного числа
    int i = 0;
    if(num[0] == '\0') return 0; //пустая строка
    for(; num[i]; i++){
        if(is_alpha(num[i]) && (num[i] < 'a') && (num[i] - 'A' + 10) < osn) continue;
        else if(is_alpha(num[i]) && (num[i] - 'a' + 10) < osn) continue;
        else if(is_digit(num[i]) && (num[i] - '0') < osn) continue;
        else return 1; //не буквы и не цифры
    }
    return 0;
}


void kaprekar(int base, int n, ...){
    if(base < 2 || base > 36){
        fprintf(stderr, "Основание СС должно быть в диапазоне [2; 36]\n");
        return;
    }
    va_list cur;
    va_start(cur, n);
    for(int i = 0; i < n; i++){
        char *number = va_arg(cur, char*);
        if(validate(number, base) == 1){
            fprintf(stderr, "Неправильно введенное число: %s\n", number);
            continue;
        }
        long number_10 = to_des(base, number);
        char number_sq[strlen(number) * 2 + 2];
        char * sq_str = des_to_system(base, number_10 * number_10, number_sq, sizeof(number_sq)); //квадрат числа в заданной сс
        int sq_len = (int)strlen(sq_str);
        if(sq_len < 2){
            continue;
        }
        for(int k = 1; k < sq_len; k++){
            char del = number_sq[k];
            number_sq[k] = '\0'; //делим строку по к-тому элементу
            long part1 = to_des(base, sq_str);
            number_sq[k] = del;
            long part2 = to_des(base, &sq_str[k]);
            if(part1 + part2 == number_10){
                printf("%s - число Капрекара в СС с основанием %d.\n", number, base);
                break;
            }
        }
    }
    va_end(cur);
}

double middle(double eps, int n, ...){
    if (n <= 0){
        fprintf(stderr, "Необходимо ввести минимум 1 число\n");
        return 0.0; 
    }
    va_list cur;
    va_start(cur, n);
    double mult = 1.0;
    for(int i = 0; i < n; i++){
        mult *= va_arg(cur, double);
    }
    if(mult == 0) return 0.0;
    double mid;
    double a = 0.0;
    double b = mult + 1; //на случай mult < 1
    while(b - a > eps){ //бинарный поиск корня н-ной степени
        mid = (b + a) / 2;
        if(pow(mid, n) > mult) b = mid;
        else a = mid;
    }
    va_end(cur);
    return a;
}

double d_pow(double x, int n){
    if(n == 0) return 1.0;
    if(n < 0) return 1 / d_pow(x, -n);
    if(n % 2 == 1) return x * d_pow(x, n - 1);
    else {
        double half = d_pow(x, n / 2);
        return half * half;
    }
}

double f1(double x) { return x*x - 2; }
double f2(double x) { return x * x - 1;}

void solutions(double a, double b, double eps, double (* f)(double)){
    double c = (a + b) / 2;
    if(f(a) * f(b) > 0){
        return ;
    }

    if(b - a <= eps) return;

    if(fabs(f(c)) <= eps){
        printf("%lf\n", c);
        return;
    }

    if(f(a) * f(c) < 0) solutions(a, c, eps, f);
    else solutions(c, b, eps, f);
}



int main(){
    printf("%d\n", is_convex(5, -1, 2, 1.52, 3.68, 4.86, 3.42, 6.18, 3.1, 7.14, 2.16));
    printf("%lf\n", polyn(2.0, 2, 1.0, -3.0, 1.0));
    kaprekar(10, 4, "9", "45", "10", "12");
    kaprekar(16, 3, "ff", "10", "9");
    kaprekar(2, 4, "1", "11", "111", "1000");
    printf("%f\n", middle(1e-9, 3, 2.0, 2.0, 2.0));
    printf("%lf\n", d_pow(2.0, -3));
    solutions(-2, 0, 1e-9, f2);
    return 0;
}
