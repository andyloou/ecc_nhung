#ifndef ECC_OPERATIONS_H
#define ECC_OPERATIONS_H

#include <ap_int.h>
#include "gf_math.h"

struct Point {
    ap_uint<256> x;
    ap_uint<256> y;
    bool infinity;
};


Point point_add_safe(const Point &P, const Point &Q);
void scalar_mul(ap_uint<256> k, Point P, Point &R_out);

#endif // ECC_OPERATIONS_H
