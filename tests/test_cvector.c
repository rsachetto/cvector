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

real rand_real(real low, real high) {
  real d;

  d = (real) rand() / ((real) RAND_MAX + 1);
  return (low + d * (high - low));
}


Test(new, empty_new) {
	size_t N = 10;
	cvec v = new_cvec(N);
	cr_assert_eq(cvec_len(v), N);

	cvec_free(v);
}

Test(new, new_with_const) {
	size_t N = 10;
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
	
	size_t N = 15;
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
	
	size_t N = 15;
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
	
	size_t N = 15;
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
