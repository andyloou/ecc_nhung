#include "gf_math.h"


ap_uint<256> mod_add(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod) {
#pragma HLS INLINE
    ap_uint<257> sum = a + b;
    return (sum >= mod) ? (ap_uint<256>)(sum - mod) : (ap_uint<256>)sum;
}


ap_uint<256> mod_sub(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod) {
#pragma HLS INLINE
    ap_uint<256> result;
    if (a >= b) {
        result = a - b;
    } else {
        result = mod + a - b;
    }
    return result;
}


ap_uint<256> mod_mul(ap_uint<256> a, ap_uint<256> b, const ap_uint<256> &mod) {
    ap_uint<256> result = 0;
    for (int i = 0; i < 256; ++i) {
#pragma HLS PIPELINE II=1
        if (b[i]) {
            result = mod_add(result, a, mod);
        }
        a = mod_add(a, a, mod);
    }

    return result;
}




ap_uint<256> mod_pow(ap_uint<256> base, ap_uint<256> exp, const ap_uint<256> &mod) {
    ap_uint<256> result = 1;

mod_pow_loop:
    for (int i = 0; i < 256; ++i) {
        if (exp[i] == 1) {
            result = mod_mul(result, base, mod);
        }
        base = mod_mul(base, base, mod);
    }
    return result;
}

ap_uint<256> mod_inv(ap_uint<256> a, const ap_uint<256> &mod) {
    ap_uint<256> t = 0, new_t = 1;
    ap_uint<256> r = mod, new_r = a;

mod_inv_loop:
    for (int i = 0; i < 256; ++i) {
        #pragma HLS PIPELINE II=1
        #pragma HLS STREAM variable=new_t depth=4
        #pragma HLS STREAM variable=new_r depth=4

        if (new_r == 0) break;

        ap_uint<256> quotient = r / new_r;

        ap_uint<256> temp_t = t - quotient * new_t;
        ap_uint<256> temp_r = r - quotient * new_r;

        t = new_t;
        new_t = temp_t;
        r = new_r;
        new_r = temp_r;
    }

    if (r > 1) return 0;
    if (t < 0) t += mod;
    return t;
}


