#ifndef __CMATRIX_H
#define __CMATRIX_H 

#include "cvector.h"

typedef  cvec * cmat;

#define cmat_rows(v) cvec_len(v)
#define cmat_cols(v) cvec_len(v[0])
#define cmat_compatible(a,b) (cmat_cols(a) == cmat_cols(b) && cmat_rows(a) == cmat_rows(b))

cmat cmat_new(size_t rows, size_t cols);
cmat cmat_new_identity(size_t n);
void cmat_scale_add(real c, cmat a, cmat b);
void cmat_free(cmat m);


#endif /* __CMATRIX_H */
