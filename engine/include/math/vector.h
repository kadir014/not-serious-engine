/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_VECTOR_H
#define _NS_VECTOR_H

#include "engine/include/math/math.h"


/**
 * @file math/vector.h
 * 
 * @brief 3D vector type and math.
 */


/**
 * @brief 3D vector type.
 */
typedef struct {
    float x; /**< X component of the vector. */
    float y; /**< Y component of the vector. */
    float z; /**< Z component of the vector. */
} nsVector3;

#define NS_VECTOR3(x, y, z) ((nsVector3){(x), (y), (z)})

static const nsVector3 nsVector3_zero = {0.0f, 0.0f, 0.0f};

static inline nsVector3 nsVector3_add(nsVector3 a, nsVector3 b) {
    return NS_VECTOR3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline nsVector3 nsVector3_sub(nsVector3 a, nsVector3 b) {
    return NS_VECTOR3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline nsVector3 nsVector3_mul(nsVector3 v, float s) {
    return NS_VECTOR3(v.x * s, v.y * s, v.z * s);
}

static inline nsVector3 nsVector3_div(nsVector3 v, float s) {
    return NS_VECTOR3(v.x / s, v.y / s, v.z / s);
}

static inline nsVector3 nvVector2_neg(nsVector3 v) {
    return NS_VECTOR3(-v.x, -v.y, -v.z);
}

static inline float nsVector3_len2(nsVector3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float nsVector3_len(nsVector3 v) {
    return ns_sqrt(nsVector3_len2(v));
}

static inline nsVector3 nsVector3_normalize(nsVector3 v) {
    return nsVector3_div(v, nsVector3_len(v));
}

static inline float nsVector3_dot(nsVector3 a, nsVector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline nsVector3 nsVector3_cross(nsVector3 a, nsVector3 b) {
    return NS_VECTOR3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

#endif