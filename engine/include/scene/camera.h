/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file scene/camera.h
 * @brief Camera.
 */
#ifndef _NS_CAMERA_H
#define _NS_CAMERA_H

#include "engine/include/_internal.h"
#include "engine/include/math/vector.h"
#include "engine/include/math/matrix.h"


typedef enum {
    nsCameraProjection_PERSPECTIVE,
    nsCameraProjection_ORTHOGRAPHIC
} nsCameraProjection;


typedef enum {
    nsCameraMode_FIRST_PERSON,
    nsCameraMode_ORBIT
} nsCameraMode;


typedef struct {
    nsCameraProjection projection;
    nsCameraMode mode;

    nsMatrix4 projection_mat;
    nsMatrix4 view_mat;

    nsVector3 position;
    nsVector3 front;
    nsVector3 up;

    float yaw;
    float pitch;

    // Only orbit camera
    nsVector3 target;
    float distance;
} nsCamera;


nsCamera *nsCamera_new(nsCameraProjection projection, float aspect_ratio);

void nsCamera_free(nsCamera *camera);

void nsCamera_update(nsCamera *camera);

void nsCamera_move(nsCamera *camera, float amount);

void nsCamera_strafe(nsCamera *camera, float amount);


#endif