#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

int solutions(double eps, double a, double b, double c, double* x1, double* x2) { //решение уравнения
	double dis = b * b - 4 * a * c;
	//линейное уравнение, чтобы избежать деления на 0
	if (fabs(a) < eps) {
		if (fabs(b) < eps && fabs(c) < eps) return -1; //уравнения вида 0*x^2 + 0*x = 0
		else if (fabs(b) < eps) return 0; //уравнения вида const = 0
		else { //bх+с=0
			*x1 = -c / b;
			return 1;
		}
	}
	if (fabs(dis) < eps) {
		*x1 = (-b / (2 * a));
		return 2;
	}
	else if (dis > 0) {
		*x1 = (-1 * b + sqrt(dis)) / (2 * a);
		*x2 = (-1 * b - sqrt(dis)) / (2 * a);
		return 3;
	}
	else return 0;
}

void print_solutions(double eps, double a, double b, double c) { //печать решений
	printf("a = %lf, b = %lf, c = %lf\n", a, b, c);
	double x1;
	double x2;
	int res = solutions(eps, a, b, c, &x1, &x2);
	switch (res) {
		case -1: //бесконечно много
			printf("Infinitely many solutions\n");
			break;
		case 0: //нет решений
			printf("No solutions.\n");
			break;
		case 1: //линейное уравнение
			printf("x = %lf\n", x1);
			break;
		case 2: //квадрат суммы (D = 0)
			printf("x1 = x2 = %lf\n", x1);
			break;
		case 3: //простое решение через D
			printf("x1 = %f, x2 = %lf\n", x1, x2);
			break;
	}
}

void flag_q(double eps, double a, double b, double c) { //решение всевозможных уравнений путем перестановки
	double coef[3] = { a, b, c };
	int permut[6][3] = { //всевозможные перестановки
		{0, 1, 2},
		{0, 2, 1},
		{1, 2, 0},
		{1, 0, 2},
		{2, 1, 0},
		{2, 0, 1}
	};
	for (int i = 0; i < 6; i++) { //проверка на повторы
		double A = coef[permut[i][0]];
		double B = coef[permut[i][1]];
		double C = coef[permut[i][2]];

		bool was_before = false;

		for (int j = 0; j < i; j++) {
			double AA = coef[permut[j][0]];
			double BB = coef[permut[j][1]];
			double CC = coef[permut[j][2]];

			if (fabs(AA - A) < eps && fabs(BB - B) < eps && fabs(CC - C) < eps) {
				was_before = true;
				break;
			}
		}
		if (!was_before) print_solutions(eps, A, B, C);
	}
}


//вычисление
int is_multiple(int a, int b) {
	if (a == 0 || b == 0) return 0;
	else if (a % b == 0) return 1;
	else return 2;
}

//вывод
void flag_m(int a, int b) {
	int m = is_multiple(a, b);
	switch (m) {
		case 0:
			fprintf(stderr, "Numbers must not be zero.\n");
			break;
		case 1:
			printf("%d is a multiple of %d\n", a, b);
			break;
		case 2:
			printf("%d is not a multiple of %d\n", a, b);
			break;
	}
}

int is_triangle(double eps, double a, double b, double c) {
	int f = (a + b - c) > eps && (a + c - b) > eps && (b + c - a) > eps;
	if (!f) return 0; //не треугольник
	else {
		int m = (fabs(a * a + b * b - c * c) < eps) || (fabs(a * a + c * c - b * b) < eps) || (fabs(b * b + c * c - a * a) < eps);
		if (m) return 1; //прямоугольный
		else return 2; //не прямоугольный
	}
}

void flag_t(double eps, double a, double b, double c) {
	int tr = is_triangle(eps, a, b, c);
	switch (tr) {
		case 0:
			printf("The three sides do not form a triangle.\n");
			break;
		case 1:
			printf("The triangle is a right triangle.\n");
			break;
		case 2:
			printf("The triangle is not a right triangle.\n");
			break;
	}
}

bool parse_4(char* argv[], double* eps, double* a, double* b, double* c) {
	int pos;
	if (sscanf(argv[2], "%lf%n", eps, &pos) != 1 || argv[2][pos] != '\0' ||
		sscanf(argv[3], "%lf%n", a, &pos) != 1 || argv[3][pos] != '\0' ||
		sscanf(argv[4], "%lf%n", b, &pos) != 1 || argv[4][pos] != '\0' ||
		sscanf(argv[5], "%lf%n", c, &pos) != 1 || argv[5][pos] != '\0') {
		return false;
	}
	return true;
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "There are not enough arguments to execute the commands.\n");
		return 1;
	}
	if (argv[1][0] == '/') argv[1][0] = '-';
	if (strcmp(argv[1], "-q") == 0) {
		double eps;
		double a;
		double b;
		double c;
		if (argc != 6) {
			fprintf(stderr, "Usage: %s -q <eps> <a> <b> <c>\n", argv[0]);
			return 1;
		}
		if (!parse_4(argv, &eps, &a, &b, &c)){
			fprintf(stderr, "Error: invalid number\n");
			return 1;
		}
		flag_q(eps, a, b, c);
	}
	else if (strcmp(argv[1], "-m") == 0) {
		if (argc != 4) {
			fprintf(stderr, "There are not enough arguments to execute the commands.\n");
			return 1;
		}
		int a;
		int b;
		int pos;
		if(sscanf(argv[2], "%d%n", &a, &pos)!= 1 || argv[2][pos] != '\0' ||
			sscanf(argv[3], "%d%n", &b, &pos) != 1 || argv[3][pos] != '\0'){
			fprintf(stderr, "Usage: %s -m <a> <b>\na, b must be int\n", argv[0]);
			return 1;
		}
		flag_m(a, b);
	}
	else if (strcmp(argv[1], "-t") == 0) {
		double eps;
		double a;
		double b;
		double c;
		if (argc != 6) {
			fprintf(stderr, "Usage: %s -t <eps> <a> <b> <c>\n", argv[0]);
			return 1;
		}
		if(!parse_4(argv, &eps, &a, &b, &c)){
			fprintf(stderr, "Error: invalid number\n");
			return 1;
		}
		flag_t(eps, a, b, c);
	}
	else {
		fprintf(stderr, "Error in spelling. Possible flags:\n-q <eps> <a> <b> <c>\n-m <a> <b>\n-t <eps> <a> <b> <c>\n");
		return 1;
	}
	return 0;
}
