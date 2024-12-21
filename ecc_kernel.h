#ifndef ECC_KERNEL_H
#define ECC_KERNEL_H

#include <ap_int.h>
#include "ecc_operations.h"
#include "gf_math.h"


const ap_uint<256> G_X = ap_uint<256>("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16);
const ap_uint<256> G_Y = ap_uint<256>("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16);

void ecc_kernel(ap_uint<256> input1, ap_uint<256> input2, ap_uint<256> input3, ap_uint<256> &output1, ap_uint<256> &output2, int mode, ap_uint<256> public_key_x = 0, ap_uint<256> public_key_y = 0);



void sign(ap_uint<256> private_key, ap_uint<256> hash, ap_uint<256> &r, ap_uint<256> &s);


bool verify(ap_uint<256> public_key_x, ap_uint<256> public_key_y, ap_uint<256> hash, ap_uint<256> r, ap_uint<256> s);

#endif // ECC_KERNEL_H
