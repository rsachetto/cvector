/* 
 * test_cmat.c
 *
 * Lets test!!
 *
 */

#include <criterion/criterion.h>
#include <signal.h>
#include <time.h>
#include "../src/cmatrix.h"
#include <stdio.h> 

#define N 2
#define M 2

real rand_real(real low, real high) {
  real d;
  d = (real) rand() / ((real) RAND_MAX + 1);
  return (low + d * (high - low));
}

Test(new, empty_new) {

	cmat m = cmat_new(N, M);

	cr_assert_eq(cmat_rows(m), N);
	cr_assert_eq(cmat_cols(m), M);

	cmat_free(m);
}

Test(new, new_identity) {

	cmat m = cmat_new_identity(N);

	cr_assert_eq(cmat_rows(m), N);
	cr_assert_eq(cmat_cols(m), N);

	FOR(i, 0, N) {
		FOR(j, 0, N) {
			if(i == j) {
				cr_assert_eq(m[i][j], REAL_CONST(1.0));
			}
			else {
				cr_assert_eq(m[i][j], REAL_CONST(0.0));
			}

		}

	}

	FOR(i, 0, N) {
		FOR(j, 0, N) {
			printf("%lf ", m[i][j]);
		}
		printf("\n");

	}
	cmat_free(m);
}

/*
Test(new, new_with_const) {
}

Test(new, cvec_from_c_array) {
}

Test(operations, cvec_prod) {

}

Test(operations, cvec_div) {
}


Test (operations, cvec_equals) {


Test(operations, dot_prod) {

}

Test(operations, max_norm) {
}

Test(operations, min) {

}

Test(operations, axpy) {
}

Test(operations, wmrs_norm) {

}
*/
