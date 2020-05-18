/* 
 * cvector.c
 *
 * Simple vector library
 *
 */

#include "cvector.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

static void *cvec_growf(void *a, size_t elemsize, size_t addlen, size_t min_cap) {

	void *b;
	size_t min_len = cvec_len(a) + addlen;

	// compute the minimum capacity needed
	if (min_len > min_cap)
		min_cap = min_len;

	if (min_cap <= cvec_cap(a))
		return a;

	// increase needed capacity to guarantee O(1) amortized
	if (min_cap < 2 * cvec_cap(a)) {
		min_cap = 2 * cvec_cap(a);
	}
	else if (min_cap < 4) {
		min_cap = 4;
	}

	b = realloc((a) ? cvec_header(a) : 0, elemsize * min_cap + sizeof(cvec_header_t));
	
	b = (char *) b + sizeof(cvec_header_t);
	if (a == NULL) {
		cvec_header(b)->length = 0;
	} 
	
	cvec_header(b)->capacity = min_cap;

	return b;
}


cvec new_cvec(size_t n) {

	cvec ret = NULL;
	cvec_setcap(ret, n);

	return ret;

}

cvec new_cvec_with_const(real c, size_t n) {

	cvec ret = new_cvec(n);

	for(size_t i = 0; i < n; i++) {
		ret[i] = c;	
	}

	return ret;

}

cvec new_cvec_from_carray(real *array, size_t n) {

	cvec res = new_cvec(n);
	memcpy(res, array, n*sizeof(real));

	return res;
}


void cvec_prod(cvec x, cvec y, cvec z) {

	size_t N;
	
	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		z[i] = x[i]*y[i];
	}

	return;
}

cvec new_cvec_from_prod(cvec x, cvec y) {

	size_t N;

	N  = cvec_len(x);
	cvec z = new_cvec(N);
	cvec_prod(x, y, z);
	
	return z;
}

void cvec_div(cvec x, cvec y, cvec z) {

	size_t N;
	
	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		z[i] = x[i]/y[i];
	}

	return;
}

cvec new_cvec_from_div(cvec x, cvec y) {

	size_t N;

	N = cvec_len(x);
	cvec z = new_cvec(N);
	cvec_div(x, y, z);
	
	return z;
}


bool cvec_equals(cvec x, cvec y) {

	size_t N1 = cvec_len(x);
	size_t N2 = cvec_len(y);

	if(N1 != N2) return false;
	if(x == y) return true;

	for(size_t i = 0; i < N1; i++) {
		if(x[i] != y[i]) return false;
	}

	return true;

}

bool cvec_equals_tol(cvec x, cvec y, real tol) {

	size_t N1 = cvec_len(x);
	size_t N2 = cvec_len(y);

	if(N1 != N2) return false;
	if(x == y) return true;

	for(size_t i = 0; i < N1; i++) {
		if(ABS(x[i] - y[i]) > tol) return false;
	}

	return true;

}

real cvector_dot_prod(cvec x, cvec y) {

  size_t N;
  real sum, *xd, *yd;

  sum = REAL_CONST(0.0);

  N  = cvec_len(x);

  for (size_t i = 0; i < N; i++) {
    sum += x[i]*y[i];
  }
  
  return sum;
}

real cvec_max_norm(cvec x) {

	size_t N;
	real max;

	max = REAL_CONST(0.0);

	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		if (ABS(x[i]) > max) max = ABS(x[i]);
	}

	return max;
}

real cvec_w_rms_norm(cvec x, cvec w) {

	size_t N;
	real sum, prod;

	sum = REAL_CONST(0.0);

	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		prod = x[i]*w[i];
		sum += SQR(prod);
	}

	return SQRT(sum/N);

}

real cvec_min(cvec x) {
  size_t N;
  real min;

  N  = cvec_len(x);

  min = x[0];

  for (size_t i = 1; i < N; i++) {
    if (x[i] < min) min = x[i];
  }

  return min;
}

real cvec_w_L2_norm(cvec x, cvec w) {

	size_t N;
	real sum, prod;

	sum = REAL_CONST(0.0);

	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		prod = x[i]*w[i];
		sum += SQR(prod);
	}

	return SQRT(sum);

}

real cvec_L1_norm(cvec x) {

	size_t N;
	real sum, prod;

	sum = REAL_CONST(0.0);

	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		sum += ABS(x[i]);
	}

	return sum;

}

void cvec_abs(cvec x, cvec z) {

	size_t N;

	N = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		z[i] = ABS(x[i]);
	}

	return;
}

cvec cvec_new_from_abs(cvec x) {

	size_t N;

	N = cvec_len(x);

	cvec z = new_cvec(N);
	cvec_abs(x, z);

	return z;
}

void cvec_inv(cvec x, cvec z) {

	assert(z);

	size_t N;

	N = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		z[i] = 1.0/x[i];
	}

	return;
}

cvec cvec_new_from_inv(cvec x) {

	size_t N;

	N = cvec_len(x);

	cvec z = new_cvec(N);
	cvec_inv(x, z);

	return z;
}

void cvec_add_const(cvec x, real b, cvec z) {

	assert(z);

	size_t N;

	N = cvec_len(x);
	for (size_t i = 0; i < N; i++) {
		z[i] = x[i] + b;
	}

	return;
}

cvec cvec_new_from_add_const(cvec x, real b) {

	size_t N;

	N = cvec_len(x);

	cvec z = new_cvec(N);
	cvec_add_const(x, b, z);

	return z;
}

void cvec_axpy(real a, cvec x, cvec y) {

	size_t N;
	size_t i;

	N  = cvec_len(x);

	if (a == REAL_CONST(1.0)) {
		for (i = 0; i < N; i++) {
			y[i] += x[i];
		}
		return;
	}

	if (a == -REAL_CONST(1.0)) {
		for (i = 0; i < N; i++) {
			y[i] -= x[i];
		}
		return;
	}    

	for (i = 0; i < N; i++) {
		y[i] += a*x[i];
	}

	return;
}

cvec cvec_new_from_axpy(real a, cvec x, cvec y) {

	size_t N;
	size_t i;

	N  = cvec_len(x);

	cvec z = new_cvec_from_carray(y, N);
	cvec_axpy(a, x, z);

	return z;
}

void cvec_scale_by(real a, cvec x) {

	size_t N;

	N  = cvec_len(x);

	for (size_t i = 0; i < N; i++) {
		x[i] *= a;
	}

	return;
}

cvec new_cvec_from_scale_by(real a, cvec x) {

	size_t N;

	N  = cvec_len(x);

	cvec z = new_cvec_from_carray(x, N);
	cvec_scale_by(a, z);

	return z;
}
