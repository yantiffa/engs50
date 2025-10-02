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
		printf("usage: program failed succesfully due to wrong number of arguments\n");
		exit(EXIT_SUCCESS);
	}
	double a = strtod(argv[1], NULL);
	double b = strtod(argv[2], NULL);
	if (b < a){
		printf("usage: program successfully failed because the upper bound can not be smaller than the lower bound\n");
		exit(EXIT_SUCCESS);
	}

	
	double res;
	uint32_t strip;
	if (strcmp(argv[3], "-n") == 0){
		uint32_t n = (uint32_t)atoi(argv[4]);
		if (n<=0){
			printf("usage: program successfully failed because n should be greater than 0\n");
			exit(EXIT_SUCCESS);
		}
		int stats = integraten(f, a,b, n, &res);
		if (stats == 0){
			printf("interval: [%lf-%lf], n: %u, result=%lf\n", a,b,n,res);
			exit(EXIT_SUCCESS);
		}
	}else if (strcmp(argv[3], "-p") == 0) {
	 double precision = strtod(argv[4], NULL);
	 if (precision <= 0 || precision > 1){
		  printf("usage: program successfully failed because p is out of range\n");                                    
     exit(EXIT_SUCCESS);
	 }
	 int stats = integratep(f, a, b, precision, &res, &strip);
	 if (stats == 0){   
			printf("interval: [%lf-%lf], n: %u, result=%lf\n", a,b,strip,res);
			exit(EXIT_SUCCESS);
	 }
	}else {
		printf("usage: program successfully failed because we no -n or -p as argument\n"); 
		exit(EXIT_SUCCESS);
	}
	printf("FAILED");
	exit(EXIT_FAILURE);
}
		
