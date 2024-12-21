#include <ap_int.h>
#include "ecc_operations.h"
#include "gf_math.h"
#include "ecc_kernel.h"

void ecc_mode_0(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &output1, ap_uint<256> &output2);
void ecc_mode_1(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> &output1, ap_uint<256> &output2);
void ecc_mode_2(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> public_key_x, ap_uint<256> public_key_y, ap_uint<256> &output1);
void ecc_dispatch(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &temp_output1, ap_uint<256> &temp_output2, int mode, ap_uint<256> public_key_x, ap_uint<256> public_key_y);

void ecc_kernel(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &output1, ap_uint<256> &output2, int mode, ap_uint<256> public_key_x, ap_uint<256> public_key_y) {

#pragma HLS INTERFACE axis port=input1
#pragma HLS INTERFACE axis port=input2
#pragma HLS INTERFACE axis port=input3
#pragma HLS INTERFACE axis port=output1
#pragma HLS INTERFACE axis port=output2
#pragma HLS INTERFACE s_axilite port=mode
#pragma HLS INTERFACE s_axilite port=public_key_x
#pragma HLS INTERFACE s_axilite port=public_key_y
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS DATAFLOW

    ap_uint<256> temp_output1, temp_output2;


    ecc_dispatch(input1, input2, input3, temp_output1, temp_output2, mode, public_key_x, public_key_y);

    output1 = temp_output1;
    output2 = temp_output2;
}

void ecc_dispatch(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &temp_output1, ap_uint<256> &temp_output2, int mode, ap_uint<256> public_key_x, ap_uint<256> public_key_y) {
#pragma HLS INLINE
    if (mode == 0) {
        ecc_mode_0(input1, input2, input3, temp_output1, temp_output2);
    } else if (mode == 1) {
        ecc_mode_1(input1, input2, temp_output1, temp_output2);
    } else if (mode == 2) {
        ecc_mode_2(input1, input2, input3, public_key_x, public_key_y, temp_output1);
    }
}


void ecc_mode_0(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &output1, ap_uint<256> &output2) {
#pragma HLS INLINE
    Point P = {input2, input3, false};
    Point R;
    scalar_mul(input1, P, R);
    output1 = R.x;
    output2 = R.y;
}

void ecc_mode_1(ap_uint<256> private_key, ap_uint<256> hash, ap_uint<256> &output1, ap_uint<256> &output2) {
#pragma HLS INLINE
    ap_uint<256> k = 123456789;
    Point R;
    scalar_mul(k, {G_X, G_Y, false}, R);
    output1 = R.x % N;
    ap_uint<256> k_inv = mod_inv(k, N);
    output2 = mod_mul(k_inv, mod_add(hash, mod_mul(output1, private_key, N), N), N); // s
}


void ecc_mode_2(ap_uint<256> r, ap_uint<256> s, ap_uint<256> hash, ap_uint<256> public_key_x, ap_uint<256> public_key_y, ap_uint<256> &output1) {
#pragma HLS INLINE
    ap_uint<256> s_inv = mod_inv(s, N);
    ap_uint<256> u1 = mod_mul(hash, s_inv, N);
    ap_uint<256> u2 = mod_mul(r, s_inv, N);

    Point P1, P2, P;
    scalar_mul(u1, {G_X, G_Y, false}, P1);
    scalar_mul(u2, {public_key_x, public_key_y, false}, P2);
    P = point_add_safe(P1, P2);

    output1 = (P.x % N == r);
}
