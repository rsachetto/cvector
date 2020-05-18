/* 
 * test_cvector.c
 *
 * Lets test!!
 *
 */

#include <criterion/criterion.h>
#include <signal.h>
#include "../src/cvector.h"
#include <time.h>

#define N 20

real rand_real(real low, real high) {
  real d;

  d = (real) rand() / ((real) RAND_MAX + 1);
  return (low + d * (high - low));
}


Test(new, empty_new) {
	
	cvec v = new_cvec(N);
	cr_assert_eq(cvec_len(v), N);

	cvec_free(v);
}

Test(new, new_with_const) {
	
	real c = REAL_CONST(2.0);
	cvec v = new_cvec_with_const(c, N);
	
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

	cvec v = new_cvec_from_carray(a, N);

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


	cvec va = new_cvec(N);
	cvec vb = new_cvec(N);
	cvec vc = new_cvec(N);

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

	vc = new_cvec_from_prod(va, vb);
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


	cvec va = new_cvec(N);
	cvec vb = new_cvec(N);
	cvec vc = new_cvec(N);

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

	vc = new_cvec_from_div(va, vb);
	cr_assert_eq(cvec_len(vc), N);

	for(i = 0; i < N; i++) {
		cr_assert_float_eq(vc[i], a[i]/b[i], 1e-10,  "Found %lf, Expected %lf.", vc[i], a[i]/b[i]);
	}
}


Test (operations, cvec_equals) {

	cvec a = new_cvec(N);
	cvec b = new_cvec(N);

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

	a = new_cvec_with_const(c, N);
	b = new_cvec_from_carray(a, N);

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
		
	cvec a = new_cvec_from_carray(a1, N);
	cvec b = new_cvec_from_carray(b1, N);

	real dp2 = cvector_dot_prod(a, b);

	cr_assert_float_eq(dp1, dp2, 1.0e-12, "Found %lf, Expected %lf.", dp1, dp2);


}

Test(operations, max_norm) {

	cvec a = new_cvec(N);

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

	cvec a = new_cvec(N);

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

