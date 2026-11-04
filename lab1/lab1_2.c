#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

double xn(int n){
    if(n == 0) return -0.5;
    double prev = xn(n - 1);
    return (prev - prev * prev / 2 + 1);
}

double ln_fact(int n) {
	double s = 0.0;
	for(int k = 1; k <= n; k++){
        s += log(k);
    }
    return s;
}

double cmk(int m, int k){
    double res = ln_fact(m) - ln_fact(m - k) - ln_fact(k);
    return exp(res);
}

double e_series(double eps) {
	double temp = 1.0;
	double res = temp;
	int n = 1;
	while (1) {
		temp = temp / n;
        if(temp < eps) break;
		res += temp;
		n++;
	}
	return res;
}

double pi_series(double eps) {
	double temp = eps + 1;
	double res = 0.0;
	int n = 1;
	while (1) {
        if(n%2 == 0) temp = -1.0 / (2 * n - 1);
		else temp = 1.0 / (2 * n - 1);
        if(fabs(temp) < eps) break;
		res += temp;
		n++;
	}
	return res * 4.0;
}

double ln_series(double eps){
    double temp = eps + 1;
    double res = 0.0;
    int n = 1;
    while(1){
        if(n % 2 == 0) temp = -1.0 / n;
        else temp = 1.0 / n;
        if(fabs(temp) < eps) break;
        res += temp;
        n++;
    }
    return res;
}

double sq_series(double eps){
    double temp = eps + 2;
    double res = 1.0;
    int n = 2;
    while(1){
        temp = pow(2.0, pow(2, -n));
        if((fabs(temp) - 1) < eps) break;
        res *= temp;
        n++;
    }
    return res;
}

double gamma_series(double eps) {
    double res = 0.0;
    int m = 1;  // номер блока
    
    while (1) {
        double block_sum = 0.0; //суммируем блоками
        int k_start = m * m;
        int k_end = (m + 1) * (m + 1) - 1;
        
        for (int k = k_start; k <= k_end; k++) {
            block_sum += 1.0 / (m * m) - 1.0 / k;
        }
        
        if (fabs(block_sum) < eps) {
            break;
        }
        
        res += block_sum;
        m++;
    }
    
    double pi = pi_series(eps);
    return - pi * pi / 6.0 + res;
}

double e_lim(double eps) {
	double prev = 0;
	double temp = eps + 1;
	int n = 1;
	while (fabs(temp - prev) > eps) {
		prev = temp;
		n *= 2;
		temp = pow(1.0 + 1.0 / n, n);
	}
	return temp;
}

double pi_lim(double eps){
    int n = 1;
    double prev = 0;
    double temp = 0;
    while(1){
        double ln_temp = 4.0 * n * log(2) + 4.0 * ln_fact(n) - log(n) - 2.0 * ln_fact(2*n);
        prev = temp;
        temp = exp(ln_temp);
        if(fabs(prev - temp) < eps) break;
        n *= 2;
    }
    return temp;
}

double ln_lim(double eps){
    long n = 1;
    double prev = 0;
    double temp = 0;
    while(1){
        prev = temp;
        temp = (double)n * (pow(2.0, 1.0/n) - 1.0);
        if(fabs(prev - temp) < eps) break;
        n*=2;
    }
    return temp;
}


double sq_lim(double eps){
    int n = 1;
    double prev;
    double temp = 0;
    while(1){
        prev = temp;
        temp = xn(n);
        if(fabs(prev - temp) < eps) break;
        n *= 2;
    }
    return temp;
}

double gamma_lim(double eps){
    double prev;
    double temp = 0;
    int m = 2;
    while(1){
        prev = temp;
        temp = 0.0;
        for(int k = 2; k <= m; k++){
            double ln_temp;
            double sign = (k % 2 == 0) ? 1.0 : -1.0;
            ln_temp = log(cmk(m, k)) + log(ln_fact(k)) - log(k) + log(1);
            temp += exp(ln_temp) * sign;            
        }
        if(fabs(temp - prev) < eps) break;
        m ++;
    }
    return temp;
}

double e_eq(double eps) {
	double a;
	double b = 1;
	double mid;
	while (log(b) < 1) {
		b++;
	}
	a = b - 1;
	while ((b - a) > eps) {
		mid = (a + b) / 2;
		if (1 > log(mid)) a = mid;
		else if (1 < log(mid)) b = mid;
		else return mid;
	}
	return (a + b) / 2;
}

double ln_eq(double eps){
    double a;
    double b = 0;
    double mid;
    while(exp(b) < 2.0){
        b ++;
    }
    a = b - 1;
    while((b - a) > eps){
        mid = (a + b) / 2;
        if(2.0 > exp(mid)) a = mid;
        else b = mid; 
    }
    return (a + b) / 2.0;
}

double sq_eq(double eps){
    double a;
    double b = 0;
    double mid;
    while(b * b < 2){
        b++;
    }
    a = b - 1;
    while((b - a) > eps){
        mid = (a + b) / 2.0;
        if(2.0 > (mid * mid)) a = mid;
        else b = mid;
    }
    return (a + b) / 2;
}


double gamma_lim_e(double eps){
    long t_max = 10000000;
    
    char *is_prime = malloc((t_max + 1) * sizeof(char));
    for (long long i = 0; i <= t_max; i++) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (long long i = 2; i * i <= t_max; i++)
        if (is_prime[i])
            for (long long j = i * i; j <= t_max; j += i)
                is_prime[j] = 0;
    
    double log_prod = 0.0;             
    double prev = 0.0, temp = 0.0;
    
    for (long t = 2; t <= t_max; t++) {
        if (is_prime[t])         
            log_prod = log_prod + log((double)(t - 1)) - log((double)t);
        
        prev = temp;
        temp = log((double)t) * exp(log_prod); 
        
        if (fabs(temp - prev) < eps) break;
    }
    
    free(is_prime);
    return temp;
}

double gamma_eq(double eps){
    double gamma_e = gamma_lim_e(eps);
    double a = 0.0;
    double b = 1.0;
    double mid;
    while((b - a) > eps){
        mid = (a + b) / 2.0;
        if(gamma_e > exp(-mid)) b = mid;
        else a = mid;
    }
    return (a + b) / 2.0;
}

double pi_eq(double eps){
    double a = 0.0;
    double b = 1.0;

    while(cos(a/2.0) * cos(b/2.0) > 0.0){
        a = b; 
        b++;
    }

    double mid;
    while(b - a > eps){
        mid = (a + b) / 2;
        if(cos(a / 2) * cos(mid / 2) > 0.0){
            a = mid;
        }
        else b = mid;
    }
    return (a + b) / 2;
}


int main() {
    printf("PI\n");
    printf("%f\n", pi_lim(1e-6));
	printf("%f\n", pi_series(1e-6));
    printf("%f\n", pi_eq(1e-6));
    printf("\n");

    printf("e\n");
    printf("%f\n", e_lim(1e-6));
	printf("%f\n", e_series(1e-6));
    printf("%f\n", e_eq(1e-6));
    printf("\n");

    printf("ln2\n");
    printf("%f\n", ln_lim(1e-6));
    printf("%f\n", ln_series(1e-6));
    printf("%f\n", ln_eq(1e-6));
    printf("\n");

    printf("sqrt2\n");
    printf("%f\n", sq_lim(1e-6));
    printf("%f\n", sq_series(1e-6));
    printf("%f\n", sq_eq(1e-6));
    printf("\n");

    printf("gamma\n");
    printf("%f\n", gamma_lim(1e-5));
    printf("%f\n", gamma_series(1e-6));
    printf("%f\n", gamma_eq(1e-6));
    printf("\n");
}
