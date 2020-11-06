/* 
 * cvector.h
 *
 * Simple vector library
 *
 */

#ifndef __CVECTOR_H
#define __CVECTOR_H 

#include <stddef.h>
#include <stdbool.h>

#define FOR(var, start, end) for(size_t (var) = (start); (var) < (end); (var)++)

#if defined(CVEC_PRECISION_DOUBLE)
	typedef double real;
	# define REAL_CONST(x) x##F
	# define MAX_REAL FLT_MAX
	# define MIN_REAL FLT_MIN
#elif defined(CVEC_PRECISION_SINGLE)
	typedef float real;
	# define REAL_CONST(x) x##F
	# define MAX_REAL DBL_MAX
	# define MIN_REAL DBL_MIN
#elif defined(CVEC_PRECISION_EXT)
	typedef long double real;
	# define REAL_CONST(x) x##L
	# define MAX_REAL LDBL_MAX
	# define MIN_REAL LDBL_MIN
#elif defined(CVEC_PRECISION_EXT)

#endif

#if defined(CVEC_PRECISION_DOUBLE)
	#define SQRT sqrt
#elif defined(CVEC_PRECISION_SINGLE)
	#define SQRT sqrtf
#elif defined(CVEC_PRECISION_EXT)
	#define SQRT sqrtl
#endif

#if defined(CVEC_PRECISION_DOUBLE)
	#define SQR(x) pow(x, 2.0)
#elif defined(CVEC_PRECISION_SINGLE)
	#define SQR(x) powf(x, 2.0)
#elif defined(CVEC_PRECISION_EXT)
	#define SQR(x) powl(x, 2.0)
#endif

#if defined(CVEC_PRECISION_DOUBLE)
	#define POW pow
#elif defined(CVEC_PRECISION_SINGLE)
	#define POW powf
#elif defined(CVEC_PRECISION_EXT)
	#define POW pow
#endif

#if defined(CVEC_PRECISION_DOUBLE)
	#define ABS fabs
#elif defined(CVEC_PRECISION_SINGLE)
	#define ABS fabsf
#elif defined(CVEC_PRECISION_EXT)
	#define ABS fabsl
#endif

typedef real * cvec;

typedef struct {
  size_t length;
  size_t capacity;
} cvec_header_t;

#define cvec_header(v)       ((cvec_header_t *) (v) - 1)
#define cvec_len(v)          ((v) ? (ptrdiff_t) cvec_header(v)->length : 0)
#define cvec_cap(v)          ((v) ? cvec_header(v)->capacity : 0)
#define cvec_grow(a,b,c)     ((a) = cvec_growf((a), sizeof *(a), (b), (c)))
#define cvec_setcap(v,n)     (cvec_grow(v,0,n), cvec_header(v)->length = n)
#define cvec_free(v)         ((void) ((v) ? free(cvec_header(v)) : (void)0), (v)=NULL)
#define cvec_append(v, r)    ((v) = cvec_appendf(v, r))

cvec cvec_new(size_t n);
cvec cvec_new_with_const(real c, size_t n);
cvec cvec_new_from_c_array(real *array, size_t n);
void cvec_prod(cvec x, cvec y, cvec z);
cvec cvec_new_from_prod(cvec x, cvec y);
void cvec_div(cvec x, cvec y, cvec z);
cvec cvec_new_from_div(cvec x, cvec y);
bool cvec_equals(cvec x, cvec y);
bool cvec_equals_tol(cvec x, cvec y, real tol);
real cvector_dot_prod(cvec x, cvec y);
real cvec_max_norm(cvec x);
real cvec_w_rms_norm(cvec x, cvec w);
real cvec_min(cvec x);
real cvec_w_L2_norm(cvec x, cvec w);
real cvec_L1_norm(cvec x);
void cvec_abs(cvec x, cvec z);
cvec cvec_new_from_abs(cvec x);
void cvec_inv(cvec x, cvec z);
cvec cvec_new_from_inv(cvec x);
void cvec_add_const(cvec x, real b, cvec z);
cvec cvec_new_from_add_const(cvec x, real b);
void cvec_axpy(real a, cvec x, cvec y);
cvec cvec_new_from_axpy(real a, cvec x, cvec y);
void cvec_scale_by(real a, cvec x);
cvec cvec_new_from_scale_by(real a, cvec x);
cvec cvec_new_from_copy(cvec src);
cvec cvec_appendf(cvec v, real r);

#endif /* __CVECTOR_H */
