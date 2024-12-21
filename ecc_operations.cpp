#include "ecc_operations.h"

Point point_add_safe(const Point &P, const Point &Q) {
    if (P.infinity) return Q;
    if (Q.infinity) return P;

    ap_uint<256> delta_y = mod_sub(Q.y, P.y, ::P);
    ap_uint<256> delta_x = mod_sub(Q.x, P.x, ::P);
    ap_uint<256> inv_delta_x = mod_inv(delta_x, ::P);
    ap_uint<256> lambda = mod_mul(delta_y, inv_delta_x, ::P);

    ap_uint<256> lambda_sq = mod_mul(lambda, lambda, ::P);
    ap_uint<256> xr = mod_sub(mod_sub(lambda_sq, P.x, ::P), Q.x, ::P);
    ap_uint<256> yr = mod_sub(mod_mul(lambda, mod_sub(P.x, xr, ::P), ::P), P.y, ::P);

    return Point{xr, yr, false};
}

void scalar_mul(ap_uint<256> k, Point P, Point &R_out) {
    Point R = {0, 0, true};
    Point Q = P;

    Point R_next, Q_next;

scalar_mul_loop:
    for (int i = 255; i >= 0; --i) {
        bool bit = k[i];

        if (bit) {
            R_next = point_add_safe(R, Q);
        } else {
            R_next = R;
        }

        Q_next = point_add_safe(Q, Q);

        R = R_next;
        Q = Q_next;
    }
    R_out = R;
}
