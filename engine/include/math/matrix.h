/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_MATRIX_H
#define _NS_MATRIX_H

#include "engine/include/math/math.h"
#include "engine/include/math/vector.h"


/**
 * @file math/matrix.h
 * 
 * @brief Matrix types and math.
 */


/**
 * @brief 4x4 matrix in column-major order.
 */
typedef struct {
    float m[16];
} nsMatrix4;


/**
 * @brief Initialize nsMatrix4 literal.
 * 
 * @param m Array of elements in column-major order.
 * @return nsMatrix4
 */
#define NS_MATRIX4(m) ((nsMatrix4){{m}})


/**
 * @brief Constant 4x4 identity matrix.
 */
static const nsMatrix4 nsMatrix4_identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

/**
 * @brief Constant 4x4 zero matrix.
 */
static const nsMatrix4 nsMatrix4_zero = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
};


#define nsMatrix4_get(mat, row, col) (mat.m[(col) * 4 + (row)])

#define nsMatrix4_set(mat, row, col, value) (mat.m[(col) * 4 + (row)] = (value))


/**
 * @brief Multiply two 4x4 matrices.
 * 
 * @param a Left-hand matrix
 * @param b Right-hand matrix
 * @return nsMatrix4 
 */
static inline nsMatrix4 nsMatrix4_mul(nsMatrix4 a, nsMatrix4 b) {
    nsMatrix4 result = nsMatrix4_zero;

    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }

    return result;
}

static inline nsMatrix4 nsMatrix4_perspective(
    float fov,
    float aspect,
    float z_near,
    float z_far
) {
    nsMatrix4 mat = nsMatrix4_zero;

    float tan_half = ns_tan(fov * 0.5f);

    mat.m[0] = 1.0f / (aspect * tan_half);
    mat.m[5] = 1.0f / tan_half;
    mat.m[10] = -(z_far + z_near) / (z_far - z_near);
    mat.m[11] = -1.0f;
    mat.m[14] = -(2.0f * z_far * z_near) / (z_far - z_near);

    return mat;
}

static inline nsMatrix4 nsMatrix4_translate(nsMatrix4 mat, nsVector3 vec) {
    mat.m[12] += vec.x;
    mat.m[13] += vec.y;
    mat.m[14] += vec.z;
    return mat;
}

static inline nsMatrix4 nsMatrix4_look_at(
    nsVector3 position,
    nsVector3 target,
    nsVector3 up
) {
    nsVector3 f = nsVector3_normalize(nsVector3_sub(target, position));
    nsVector3 s = nsVector3_normalize(nsVector3_cross(f, up));
    nsVector3 u = nsVector3_cross(s, f);

    nsMatrix4 result = nsMatrix4_identity;
    result.m[0] = s.x;  result.m[4] = s.y;  result.m[8] = s.z;   result.m[12] = -nsVector3_dot(s, position);
    result.m[1] = u.x;  result.m[5] = u.y;  result.m[9] = u.z;   result.m[13] = -nsVector3_dot(u, position);
    result.m[2] = -f.x; result.m[6] = -f.y; result.m[10] = -f.z; result.m[14] = nsVector3_dot(f, position);

    return result;
}

#endif