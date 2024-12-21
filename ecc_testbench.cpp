#include <iostream>
#include "ecc_operations.h"
#include "gf_math.h"
#include "ecc_kernel.h"

int main() {
    ap_uint<256> input1, input2, input3;
    ap_uint<256> output1, output2;
    ap_uint<256> expected_output1, expected_output2;
    int mode;

    ap_uint<256> public_key_x = G_X;
    ap_uint<256> public_key_y = G_Y;

    std::cout << "Testing Mode 0: Scalar Multiplication" << std::endl;
    input1 = 123456;
    input2 = G_X;
    input3 = G_Y;
    mode = 0;

    ecc_kernel(input1, input2, input3, output1, output2, mode, public_key_x, public_key_y);

    Point expected_point;
    scalar_mul(input1, {input2, input3, false}, expected_point);
    expected_output1 = expected_point.x;
    expected_output2 = expected_point.y;

    if (output1 == expected_output1 && output2 == expected_output2) {
        std::cout << "Mode 0 Passed!" << std::endl;
    } else {
        std::cout << "Mode 0 Failed!" << std::endl;
        std::cout << "Expected Output1: " << expected_output1 << ", Got: " << output1 << std::endl;
        std::cout << "Expected Output2: " << expected_output2 << ", Got: " << output2 << std::endl;
    }

    std::cout << "\nTesting Mode 1: ECDSA Signature Generation" << std::endl;
    input1 = 123456;
    input2 = 789012;
    mode = 1;

    ecc_kernel(input1, input2, 0, output1, output2, mode, public_key_x, public_key_y);

    ap_uint<256> k = 123456789;
    Point R;
    scalar_mul(k, {G_X, G_Y, false}, R);
    expected_output1 = R.x % N;
    ap_uint<256> k_inv = mod_inv(k, N);
    expected_output2 = mod_mul(k_inv, mod_add(input2, mod_mul(expected_output1, input1, N), N), N);

    if (output1 == expected_output1 && output2 == expected_output2) {
        std::cout << "Mode 1 Passed!" << std::endl;
    } else {
        std::cout << "Mode 1 Failed!" << std::endl;
        std::cout << "Expected Output1: " << expected_output1 << ", Got: " << output1 << std::endl;
        std::cout << "Expected Output2: " << expected_output2 << ", Got: " << output2 << std::endl;
    }

    std::cout << "\nTesting Mode 2: ECDSA Signature Verification" << std::endl;
    input1 = 112233;
    input2 = 445566;
    input3 = 789012;
    mode = 2;

    ecc_kernel(input1, input2, input3, output1, output2, mode, public_key_x, public_key_y);

    ap_uint<256> s_inv = mod_inv(input2, N);
    ap_uint<256> u1 = mod_mul(input3, s_inv, N);
    ap_uint<256> u2 = mod_mul(input1, s_inv, N);

    Point P1, P2, P;
    scalar_mul(u1, {G_X, G_Y, false}, P1);
    scalar_mul(u2, {public_key_x, public_key_y, false}, P2);
    P = point_add_safe(P1, P2);

    expected_output1 = (P.x % N == input1);

    if (output1 == expected_output1) {
        std::cout << "Mode 2 Passed!" << std::endl;
    } else {
        std::cout << "Mode 2 Failed!" << std::endl;
        std::cout << "Expected Output1: " << expected_output1 << ", Got: " << output1 << std::endl;
    }

    return 0;
}
