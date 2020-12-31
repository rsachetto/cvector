
#include "cmatrix.h"
#include <stdlib.h>

cmat cmat_new(size_t rows, size_t cols) {

	cmat res = NULL;

	cvec_setcap(res, rows);

	FOR(i, 0, rows) {
		res[i] = cvec_new(cols);
	}

	return res;

}

void cmat_free(cmat m) {
	int rows = cmat_rows(m);

	FOR(i, 0, rows) {
		cvec_free(m[i]);
	}

	cvec_free(m);
}

cmat cmat_new_identity(size_t n) {
	cmat res = cmat_new(n, n);

	FOR(i, 0, n) {
		FOR(j, 0, n) {
			if(i == j) {
				res[i][j] = ONE;
			}
			else {
				res[i][j] = ZERO;
			}
		}
	}	

	return res;

}

void cmat_scale_add(real c, cmat a, cmat b) {

	if(!cmat_compatible(a, b)) {
		//TODO: make it return an error???
		return;
	}

	size_t rows, cols;
	rows = cmat_rows(a);
	cols = cmat_cols(a);

	FOR(i, 0, rows) {
		FOR(j, 0, cols) {
			a[i][j] = c*a[i][j] + b[i][j];
		}		
	}

}

cmat cmat_new_from_scale_add(real c, cmat a, cmat b) {

	cmat r = cmat_new(cmat_rows(a), cmat_cols(a));
	//TODO: create the copy of a
	//r = cmat_copy(a);
	cmat_scale_add(c, r, b);

	return r;

}
