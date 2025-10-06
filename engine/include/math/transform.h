/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file math/transform.h
 * @brief 3D transform type.
 */
#ifndef _NS_TRANSFORM_H
#define _NS_TRANSFORM_H

#include "engine/include/math/vector.h"
#include "engine/include/math/matrix.h"


/**
 * @brief 3D transform type.
 */
typedef struct {
    nsVector3 position; /**< Translation of the transform. */
    nsVector3 rotation; /**< Rotation of the transform as euler angles. */
    nsVector3 scale; /**< Scale of the transform. */
} nsTransform;

/**
 * @brief Constant zero transform.
 * 
 * @warning that scale is not 0 but 1!
 */
static const nsTransform nsTransform_zero = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}
};


static inline nsMatrix4 nsTransform_to_matrix4(nsTransform xform) {
    nsMatrix4 mat = nsMatrix4_identity;

    // Rotation -> Scale -> Translation

    // Prepare sin and cos for euler angles
    float cx = ns_cos(xform.rotation.x);
    float sx = ns_sin(xform.rotation.x);
    float cy = ns_cos(xform.rotation.y);
    float sy = ns_sin(xform.rotation.y);
    float cz = ns_cos(xform.rotation.z);
    float sz = ns_sin(xform.rotation.z);

    // Rotation order: Z * Y * X
    mat.m[0] = cy * cz;
    mat.m[4] = -cy * sz;
    mat.m[8] = sy;

    mat.m[1] = sx * sy * cz + cx * sz;
    mat.m[5] = -sx * sy * sz + cx * cz;
    mat.m[9] = -sx * cy;

    mat.m[2] = -cx * sy * cz + sx * sz;
    mat.m[6] = cx * sy * sz + sx * cz;
    mat.m[10] = cx * cy;

    // Apply scale
    mat.m[0] *= xform.scale.x;
    mat.m[1] *= xform.scale.x;
    mat.m[2] *= xform.scale.x;

    mat.m[4] *= xform.scale.y;
    mat.m[5] *= xform.scale.y;
    mat.m[6] *= xform.scale.y;

    mat.m[8] *= xform.scale.z;
    mat.m[9] *= xform.scale.z;
    mat.m[10] *= xform.scale.z;

    // Translate
    mat.m[12] = xform.position.x;
    mat.m[13] = xform.position.y;
    mat.m[14] = xform.position.z;

    return mat;
}


#endif