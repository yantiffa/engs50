#pragma once   /* allow this file to be included multiple times */

/*
 * integrate.h -- module header for the integrate module
 */

#include <inttypes.h>						/* needed for this module */

/*
 * integratep -- integrates a function f over the interval [a,b] to
 * within precision p, using trapezoidal approximation.
 *
 * Expects: b > a, 0 < p < 1.
 *
 * Returns 0 for success; non-zero otherwise 
 *
 * Produces the integral approximation by reference through a result
 * pointer (rp), and the cumulative number of strip calculations used
 * to produce the approximation through a strip pointer (sp).
 */
int32_t integratep(double (*f)(double x), 
									double a, double b, double p, 
									double *rp, uint32_t *sp);

/*
 * integraten -- integrates a function f over the interval [a,b] over
 * a specified number of divisions, n
 *
 * Expects: b > a, 0 < n <= maximum uint32_t
 *
 * Returns 0 for success; non-zero otherwise 
 *
 * Produces the integral approximation by reference through a result
 * pointer (rp).
 */
int32_t integraten(double (*f)(double x), 
									double a, double b, uint32_t n, 
									double *rp);
