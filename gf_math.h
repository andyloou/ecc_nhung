#ifndef GF_MATH_H
#define GF_MATH_H

#include <ap_int.h>

const ap_uint<256> P = ap_uint<256>("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", 16);
const ap_uint<256> N = ap_uint<256>("FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551", 16);

const ap_uint<256> P_MINUS_2 = P - 2;

ap_uint<256> mod_add(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod);
ap_uint<256> mod_sub(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod);
ap_uint<256> mod_mul(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod);
ap_uint<256> mod_inv(ap_uint<256> a, const ap_uint<256> &mod);
ap_uint<256> mod_pow(ap_uint<256> base, ap_uint<256> exp, const ap_uint<256> &mod);
ap_uint<256> fast_reduce(ap_uint<512> product, const ap_uint<256> &mod);

#endif // GF_MATH_H
