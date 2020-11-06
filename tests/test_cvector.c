/* 
 * test_cvector.c
 *
 * Lets test!!
 *
 */

#include <criterion/criterion.h>
#include <signal.h>
#include <time.h>
#include "../src/cvector.h"

#define N 20

real rand_real(real low, real high) {
  real d;

  d = (real) rand() / ((real) RAND_MAX + 1);
  return (low + d * (high - low));
}


Test(new, empty_new) {
	
	cvec v = cvec_new(N);
	cr_assert_eq(cvec_len(v), N);

	cvec_free(v);
}

Test(new, new_with_const) {
	
	real c = REAL_CONST(2.0);
	cvec v = cvec_new_with_const(c, N);
	
	cr_assert_eq(cvec_len(v), N);

	for(size_t i = 0; i < N; i++) {
		cr_assert_float_eq(v[i], c, 1e-10,  "Found %lf, Expected %lf.", v[i], c);
	}

	cvec_free(v);

}

Test(new, cvec_from_c_array) {

	srand(time(NULL));
	
	size_t i;

	real a[N];

	for(i = 0; i < N; i++) 
		a[i] = rand_real(-20, 20);

	cvec v = cvec_new_from_c_array(a, N);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(v[i], a[i], 1e-10,  "Found %lf, Expected %lf.", v[i], a[i]);
	}

	cvec_free(v);
}

Test(operations, cvec_prod) {

	srand(time(NULL));
	
	size_t i;

	real a[N];
	real b[N];

	for(i = 0; i < N; i++) 
		a[i] = rand_real(-20, 20);

	for(i = 0; i < N; i++) 
		b[i] = rand_real(-20, 20);


	cvec va = cvec_new(N);
	cvec vb = cvec_new(N);
	cvec vc = cvec_new(N);

	cr_assert_eq(cvec_len(va), N);
	cr_assert_eq(cvec_len(vb), N);
	cr_assert_eq(cvec_len(vc), N);

	for(i = 0; i < N; i++) {
		va[i] = a[i];
		vb[i] = b[i];
	}

	cvec_prod(va, vb, vc);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(vc[i], a[i]*b[i], 1e-10,  "Found %lf, Expected %lf.", vc[i], a[i]*b[i]);
	}


	cvec_free(vc);

	vc = cvec_new_from_prod(va, vb);
	cr_assert_eq(cvec_len(vc), N);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(vc[i], a[i]*b[i], 1e-10,  "Found %lf, Expected %lf.", vc[i], a[i]*b[i]);
	}
}

Test(operations, cvec_div) {

	srand(time(NULL));
	
	size_t i;

	real a[N];
	real b[N];

	for(i = 0; i < N; i++) 
		a[i] = rand_real(-20, 20);

	for(i = 0; i < N; i++) 
		b[i] = rand_real(-20, 20);


	cvec va = cvec_new(N);
	cvec vb = cvec_new(N);
	cvec vc = cvec_new(N);

	cr_assert_eq(cvec_len(va), N);
	cr_assert_eq(cvec_len(vb), N);
	cr_assert_eq(cvec_len(vc), N);

	for(i = 0; i < N; i++) {
		va[i] = a[i];
		vb[i] = b[i];
	}

	cvec_div(va, vb, vc);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(vc[i], a[i]/b[i], 1e-10,  "Found %lf, Expected %lf.", vc[i], a[i]/b[i]);
	}


	cvec_free(vc);

	vc = cvec_new_from_div(va, vb);
	cr_assert_eq(cvec_len(vc), N);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(vc[i], a[i]/b[i], 1e-10,  "Found %lf, Expected %lf.", vc[i], a[i]/b[i]);
	}
}


Test (operations, cvec_equals) {

	cvec a = cvec_new(N);
	cvec b = cvec_new(N);

	size_t i;

	for(i = 0; i < N; i++) {
		a[i] = rand_real(-20, 20);
		b[i] = rand_real(-20, 20);
	}

	if(cvec_equals(a, b)) {
		for(i = 0; i < N; i++) {
			cr_assert_float_eq(a[i], b[i], 0.0, "Found %lf, Expected %lf.", a[i], b[i]);
		}
	}

	cvec_free(a);
	cvec_free(b);
	real c = REAL_CONST(2.5);

	a = cvec_new_with_const(c, N);
	b = cvec_new_from_c_array(a, N);

	cr_assert_eq(cvec_len(a), N);
	cr_assert_eq(cvec_len(b), N);

	bool eq = cvec_equals(a, b);

	cr_assert(eq);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(a[i], b[i], 0.0, "Found %lf, Expected %lf.", a[i], b[i]);
	}

}


static real dot_product(real v[], real u[], int n)
{
    real result = 0.0;
    for (int i = 0; i < n; i++)
        result += v[i]*u[i];

	return result;
}

Test(operations, dot_prod) {

	real a1[N];
	real b1[N];

	size_t i;

	for(i = 0; i < N; i++) {
		a1[i] = rand_real(-20, 20);
		b1[i] = rand_real(-20, 20);
	}

	real dp1 = dot_product(a1, b1, N);
		
	cvec a = cvec_new_from_c_array(a1, N);
	cvec b = cvec_new_from_c_array(b1, N);

	real dp2 = cvector_dot_prod(a, b);

	cr_assert_float_eq(dp1, dp2, 1.0e-12, "Found %lf, Expected %lf.", dp1, dp2);


}

Test(operations, max_norm) {

	cvec a = cvec_new(N);

	for(size_t i = 0; i < N; i++) {
		a[i] = (real)i;
	}

	cr_assert_eq(cvec_len(a), N);

	real norm = cvec_max_norm(a);

	cr_assert_float_eq(norm, (real)N-REAL_CONST(1.0), 1.0e-12, "Found %lf, Expected %lf.", norm, (real)N-REAL_CONST(1.0));

	for(size_t i = 0; i < N; i++) {
		a[i] = -((real)i);
	}

	norm = cvec_max_norm(a);

	cr_assert_float_eq(norm, (real)N-REAL_CONST(1.0), 1.0e-12, "Found %lf, Expected %lf.", norm, (real)N-REAL_CONST(1.0));

}

Test(operations, min) {

	cvec a = cvec_new(N);

	for(size_t i = 0; i < N; i++) {
		a[i] = (real)i;
	}

	cr_assert_eq(cvec_len(a), N);

	real min = cvec_min(a);

	cr_assert_float_eq(min, REAL_CONST(0.0), 1.0e-12, "Found %lf, Expected %lf.", min, REAL_CONST(0.0));

	for(size_t i = 0; i < N; i++) {
		a[i] = -((real)i);
	}

	min = cvec_min(a);

	cr_assert_float_eq(min, -((real)N-REAL_CONST(1.0)), 1.0e-12, "Found %lf, Expected %lf.", min, -((real)N-REAL_CONST(1.0)));


}

void axpy(real a, real * x, real * y, int n) {
	for (size_t i = 0; i < n; i++) {
		y[i] = a * x[i] + y[i];
	}
}

Test(operations, axpy) {

	real a1[N];
	real b1[N];
	
	real a_xy = rand_real(-20, 20);
	size_t i;

	for(i = 0; i < N; i++) {
		a1[i] = rand_real(-20, 20);
		b1[i] = rand_real(-20, 20);
	}

	axpy(a_xy, a1, b1, N);

	cvec a = cvec_new_from_c_array(a1, N);
	cvec b = cvec_new_from_c_array(b1, N);

	axpy(a_xy, a1, b1, N);

	cvec_axpy(a_xy, a, b);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(b1[i], b[i], 0.0, "Found %lf, Expected %lf.", b1[i], b[i]);
	}

	cvec_free(a);
	cvec_free(b);

	a_xy = REAL_CONST(1.0);

	for(i = 0; i < N; i++) {
		a1[i] = rand_real(-20, 20);
		b1[i] = rand_real(-20, 20);
	}

	axpy(a_xy, a1, b1, N);

	a = cvec_new_from_c_array(a1, N);
	b = cvec_new_from_c_array(b1, N);

	axpy(a_xy, a1, b1, N);

	cvec_axpy(a_xy, a, b);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(b1[i], b[i], 0.0, "Found %lf, Expected %lf.", b1[i], b[i]);
	}

	cvec_free(a);
	cvec_free(b);

	a_xy = REAL_CONST(-1.0);

	for(i = 0; i < N; i++) {
		a1[i] = rand_real(-20, 20);
		b1[i] = rand_real(-20, 20);
	}

	axpy(a_xy, a1, b1, N);

	a = cvec_new_from_c_array(a1, N);
	b = cvec_new_from_c_array(b1, N);

	axpy(a_xy, a1, b1, N);

	cvec c = cvec_new_from_axpy(a_xy, a, b);
	cvec_axpy(a_xy, a, b);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(b1[i], b[i], 0.0, "Found %lf, Expected %lf.", b1[i], b[i]);
		cr_assert_float_eq(c[i], b[i], 0.0, "Found %lf, Expected %lf.", c[i], b[i]);
	}

}

Test(operations, wmrs_norm) {

	real result = 3.559026084010437;

	cvec x = cvec_new(3);
	x[0] = 2;
	x[1] = 3;
	x[2] = 5;

	cvec y = cvec_new_with_const(REAL_CONST(1.0), 3);

	real n = cvec_w_rms_norm(x, y);

	cr_assert_float_eq(n, result, 1e-10, "Found %lf, Expected %lf.", n, result);

}

Test(operation, append) {

	real result = 3.559026084010437;

	cvec x = NULL;

	cvec_append(x, 2.0);
	cvec_append(x, 3.0);
	cvec_append(x, 5.0);

	cvec y = cvec_new_with_const(REAL_CONST(1.0), 3);

	real n = cvec_w_rms_norm(x, y);

	cr_assert_float_eq(n, result, 1e-10, "Found %lf, Expected %lf.", n, result);


}


