/* 
 * test1.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 09-24-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include "integrate.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

double f(double x){
	double res = 0;
	res = 2*pow(x, 2) + 9*x+4;
	return res;
}

int main(int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "usage: test1.c\n");
		exit(EXIT_FAILURE);
	}
	double a = strtod(argv[1], NULL);
	double b = strtod(argv[2], NULL);
	if (b < a){
		fprintf(stderr,"usage: test1.c\n");
		exit(EXIT_FAILURE);
	}
	double res;
	uint32_t strip;
	if (strcmp(argv[3], "-n") == 0){
		uint32_t n = (uint32_t)atoi(argv[4]);
		int stats = integraten(f, a,b, n, &res);
		if (stats == 0){
			printf("interval: [%lf-%lf], n: %u, result=%lf\n", a,b,n,res);
			exit(EXIT_SUCCESS);
		}
	}else if (strcmp(argv[3], "-p") == 0) {
	 double precision = strtod(argv[4], NULL);
	 if (precision <= 0 || precision > 1) {
		 fprintf(stderr, "usgae: test1.c\n");
		 exit(EXIT_FAILURE);
	 }
	 int stats = integratep(f, a, b, precision, &res, &strip);
	 if (stats == 0){   
		  printf("interval: [%lf-%lf], n: %u, result=%lf\n", a,b,strip,res);
			exit(EXIT_SUCCESS);
	 }
	}
	fprintf(stderr, "usage: integrate.c and test1.c\n"); 
	exit(EXIT_FAILURE); 
}
		
