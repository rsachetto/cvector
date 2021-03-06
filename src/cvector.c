/*
 * cvector.c
 *
 * Simple vector library
 *
 */

#include "cvector.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void *cvec_growf(void *a, size_t elemsize, size_t addlen, size_t min_cap) {
    void *b;
    size_t min_len = cvec_len(a) + addlen;

    if(min_len > min_cap) min_cap = min_len;

    if(min_cap <= cvec_cap(a)) return a;

    if(min_cap < 2 * cvec_cap(a)) {
        min_cap = 2 * cvec_cap(a);
    } else if(min_cap < 4) {
        min_cap = 4;
    }

    b = realloc((a) ? cvec_header(a) : 0, elemsize * min_cap + sizeof(cvec_header_t));

    b = (char *)b + sizeof(cvec_header_t);

    if(a == NULL) {
        cvec_header(b)->length = 0;
    }

    cvec_header(b)->capacity = min_cap;

    return b;
}

cvec cvec_appendf(cvec v, real r) {
    int l = 0;
    int c = 0;

    if(v) {
        l = cvec_header(v)->length;
        c = cvec_header(v)->capacity;
    }

    if(l + 1 > c) cvec_grow(v, 1, 0);

    v[cvec_header(v)->length++] = r;

    return v;
}

cvec cvec_new(size_t n) {
    cvec ret = NULL;
    cvec_setcap(ret, n);
    return ret;
}

cvec cvec_new_with_const(real c, size_t n) {
    cvec ret = cvec_new(n);

    FOR(i, 0, n) {
        ret[i] = c;
    }

    return ret;
}

cvec cvec_new_from_range(real start, real end, real step) {
    cvec ret = NULL;

    FOR_S(i, start, end, step) {
		cvec_append(ret, i);
    }

    return ret;
}

cvec cvec_new_from_c_array(real *array, size_t n) {
    cvec res = cvec_new(n);
    memcpy(res, array, n * sizeof(real));

    return res;
}

cvec cvec_new_from_copy(cvec src) {
    size_t N = cvec_len(src);
    return cvec_new_from_c_array(src, N);
}

void cvec_prod(cvec x, cvec y, cvec z) {
    size_t N = cvec_len(x);

    FOR(i, 0, N) {
        z[i] = x[i] * y[i];
    }
}

cvec cvec_new_from_prod(cvec x, cvec y) {
    size_t N = cvec_len(x);

    cvec z = cvec_new(N);
    cvec_prod(x, y, z);

    return z;
}

void cvec_div(cvec x, cvec y, cvec z) {
    size_t N = cvec_len(x);

    FOR(i, 0, N) {
        z[i] = x[i] / y[i];
    }
}

cvec cvec_new_from_div(cvec x, cvec y) {
    size_t N = cvec_len(x);
    cvec z = cvec_new(N);
    cvec_div(x, y, z);

    return z;
}

bool cvec_equals(cvec x, cvec y) {
    size_t N1 = cvec_len(x);
    size_t N2 = cvec_len(y);

    if(N1 != N2) return false;
    if(x == y) return true;

    FOR(i, 0, N1) {
        if(x[i] != y[i]) return false;
    }

    return true;
}

bool cvec_equals_tol(cvec x, cvec y, real tol) {
    size_t N1 = cvec_len(x);
    size_t N2 = cvec_len(y);

    if(N1 != N2) return false;
    if(x == y) return true;

    FOR(i, 0, N1) {
        if(ABS(x[i] - y[i]) > tol) return false;
    }

    return true;
}

real cvector_dot_prod(cvec x, cvec y) {
    size_t N = cvec_len(x);
    real sum = REAL_CONST(0.0);

    FOR(i, 0, N) {
        sum += x[i] * y[i];
    }

    return sum;
}

real cvec_max_norm(cvec x) {
    size_t N = cvec_len(x);
    real max = REAL_CONST(0.0);

    FOR(i, 0, N) {
        if(ABS(x[i]) > max) max = ABS(x[i]);
    }

    return max;
}

real cvec_w_rms_norm(cvec x, cvec w) {
    size_t N = cvec_len(x);
    real sum = REAL_CONST(0.0);

    FOR(i, 0, N) {
        real prod = x[i] * w[i];
        sum += SQR(prod);
    }

    return SQRT(sum / N);
}

real cvec_min(cvec x) {
    size_t N = cvec_len(x);

    real min = x[0];

    FOR(i, 0, N) {
        if(x[i] < min) min = x[i];
    }

    return min;
}

real cvec_w_L2_norm(cvec x, cvec w) {
    size_t N = cvec_len(x);
    real sum = REAL_CONST(0.0);

    FOR(i, 0, N) {
        sum += SQR(x[i] * w[i]);
    }

    return SQRT(sum);
}

real cvec_L1_norm(cvec x) {
    size_t N = cvec_len(x);
    real sum = REAL_CONST(0.0);

    FOR(i, 0, N) {
        sum += ABS(x[i]);
    }

    return sum;
}

void cvec_abs(cvec x, cvec z) {
    size_t N = cvec_len(x);

    FOR(i, 0, N) {
        z[i] = ABS(x[i]);
    }
}

cvec cvec_new_from_abs(cvec x) {
    size_t N = cvec_len(x);

    cvec z = cvec_new(N);
    cvec_abs(x, z);

    return z;
}

void cvec_inv(cvec x, cvec z) {
    assert(z);

    size_t N = cvec_len(x);

    FOR(i, 0, N) {
        z[i] = 1.0 / x[i];
    }
}

cvec cvec_new_from_inv(cvec x) {
    size_t N = cvec_len(x);
    cvec z = cvec_new(N);

    cvec_inv(x, z);

    return z;
}

void cvec_add_const(cvec x, real b, cvec z) {
    assert(z);

    size_t N = cvec_len(x);

    FOR(i, 0, N) {
        z[i] = x[i] + b;
    }
}

cvec cvec_new_from_add_const(cvec x, real b) {
    size_t N = cvec_len(x);

    cvec z = cvec_new(N);
    cvec_add_const(x, b, z);

    return z;
}

void cvec_axpy(real a, cvec x, cvec y) {
    size_t N = cvec_len(x);

    size_t i;

    if(a == REAL_CONST(1.0)) {
        for(i = 0; i < N; i++) {
            y[i] = x[i] + y[i];
        }
        return;
    }

    if(a == -REAL_CONST(1.0)) {
        for(i = 0; i < N; i++) {
            y[i] = x[i] - y[i];
        }
        return;
    }

    for(i = 0; i < N; i++) {
        y[i] = a * x[i] + y[i];
    }

    return;
}

cvec cvec_new_from_axpy(real a, cvec x, cvec y) {
    cvec z = cvec_new_from_copy(y);
    cvec_axpy(a, x, z);

    return z;
}

void cvec_scale_by(real a, cvec x) {
    size_t N;

    N = cvec_len(x);

    FOR(i, 0, N) {
        x[i] *= a;
    }
}

cvec cvec_new_from_scale_by(real a, cvec x) {
    cvec z = cvec_new_from_copy(x);
    cvec_scale_by(a, z);

    return z;
}

void cvec_square(cvec x) {
    size_t N;

    N = cvec_len(x);

    FOR(i, 0, N) {
        x[i] = SQR(x[i]);
    }
}

cvec cvec_new_from_square(cvec x) {
    cvec z = cvec_new_from_copy(x);
    cvec_square(z);

    return z;
}
