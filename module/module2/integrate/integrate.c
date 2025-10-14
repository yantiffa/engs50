/*
 * d.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 09-23-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static double expected(double a, double b) {
	double a_res = (2/3.0)*pow(a, 3) + (9/2.0)*pow(a, 2) + 4*a;
	double b_res = (2/3.0)*pow(b, 3) + (9/2.0)*pow(b, 2) + 4*b;
	return (b_res - a_res);
}

int32_t integratep(double (*f)(double x), double a, double b, double p,double *rp, uint32_t *sp){
	double sums = 0;
	if (b<a){
		printf("usage: integrate.c");
		exit(EXIT_FAILURE);
	}
	uint32_t n = 1;
	double div = (b - a)/n;
	double slow = a;
	double fast = a + div;
	for (int i = 0; i < n; i++){
		double height1 = f(slow);
		double height2 = f(fast);
		double area = ((height1 + height2)/2) * div;
		sums += area;
		slow = fast;
		fast = fast + div;
	}
	double precision = p;
	while (fabs(sums - expected(a, b)) > precision) {
		sums = 0;
		n = n * 2;
		double div = (b - a)/n;
		double slow = a;
		double fast = a + div;
		for (int i = 0; i < n; i++){
			double height1 = f(slow);
			double height2 = f(fast);
			double area = ((height1 + height2)/2) * div;
			sums += area;
			slow = fast;
			fast = fast + div;
		}
	}
	*rp = sums;
	*sp = n;
	return 0;
}
	
	
int32_t integraten(double (*f)(double x),double a, double b, uint32_t n, double *rp){
	if (b<a){
		printf("usage: integrate.c"); 
		exit(EXIT_FAILURE);
	}
	double sums = 0;
	double div = (b - a)/n;
	double slow = a;
	double fast = a + div;
	for (int i = 0; i < n; i++){
		double height1 = f(slow);
		double height2 = f(fast);
		double area = ((height1 + height2)/2) * div;
		sums += area;
		slow = fast;
		fast = fast + div;
	}
	*rp = sums;
	return 0;
}
