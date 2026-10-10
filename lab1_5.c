#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>

double f1(double x) {
	if (fabs(x) < 1e-15) return 1.0;
	return log(1 + x) / x;
}

double f2(double x) {
	return exp(-x * x / 2);
}

double f3(double x) {
	return log(1 / (1 - x));
}

double f4(double x) {
	if (x < 1e-15) return 1.0;
	return exp(x * log(x));
}

double simpson(double (*f)(double), double a, double b, int n) {
	double res = f(a) + f(b);
	double dx = (b - a) / n;
	for (int i = 1; i < n; i ++) {
		double x = a + i * dx;
		if (i % 2 == 0) res += 2.0 * f(x);
		else res += 4.0 * f(x);
	}
	return res * dx / 3.0;
}

int main(int argc, char* argv[]) {
	double eps;
	double x;
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <x> <eps>\n", argv[0]);
		return 1;
	}
	int m = sscanf(argv[1], "%lf", &x);
	m += sscanf(argv[2], "%lf", &eps);
	if (m != 2) {
		fprintf(stderr, "Usage: %s <x> <eps>\n", argv[0]);
		return 1;
	}
	int n = 0;
	double sum = 0;
	double cur = 1.0;
	while (fabs(cur) > eps) {
		sum += cur;
		n++;
		cur = cur * x / n;
	}
	printf("a) %lf\n", sum);


	sum = 0;
	n = 0;
	cur = 1.0;
	while (fabs(cur) > eps) {
		sum += cur;
		n++;
		cur *= -x * x / ((2 * n - 1) * (2 * n));
	}
	printf("b) %lf\n", sum);


	sum = 0;
	n = 0;
	cur = 1.0;
	while (fabs(cur) > eps) {
		sum += cur;
		n++;
		cur = cur * 27 * n * n * n * x * x / ((3 * n - 2) * (3 * n - 1) * (3 * n));
	}
	printf("c) %lf\n", sum);


	sum = 0;
	n = 1;
	cur = -x * x / 2.0;
	while (fabs(cur) > eps) {
		sum += cur;
		n++;
		cur = -cur * (2 * n - 1) * x * x / (2 * n);
	}
	printf("d) %lf\n", sum);



	double (*functions[4])(double) = { f1, f2, f3, f4 };
	char names[4] = { 'a', 'b', 'c', 'd' };
	for (int k = 0; k < 4; k++) {
		n = 10;
		double a = 0;
		double b = 1;
		if (k == 2) {
			b = 1.0 - 1e-12;
		}
		double simp_cur = simpson(functions[k], a, b, n * 2);
		double simp_prev = simpson(functions[k], a, b, n);
		int iter = 0;
		const int MAX_ITER = 50;
		while (fabs(simp_cur - simp_prev) > eps && iter < MAX_ITER) {
			n *= 2;
			simp_prev = simp_cur;
			simp_cur = simpson(functions[k], a, b, n * 2);
			iter++;
		}
		if (iter >= MAX_ITER) {
			printf("%c) Не достигнута точность (результат ~%lf)\n", names[k], simp_cur);
		}
		else {
			printf("%c) %lf\n", names[k], simp_cur);
		}
	}
}


