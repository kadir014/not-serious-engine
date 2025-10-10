/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/scene/camera.h"
#include "engine/include/math/math.h"


nsCamera *nsCamera_new(nsCameraProjection projection, float aspect_ratio) {
    nsCamera *camera = NS_NEW(nsCamera);
    NS_MEM_CHECK(camera);

    camera->projection = projection;
    camera->mode = nsCameraMode_ORBIT;

    // TODO: ortho
    camera->projection_mat = nsMatrix4_perspective(
        NS_RADIANS(45.0f),
        aspect_ratio,
        0.1f,
        1000.0f
    );

    camera->position = nsVector3_zero;
    camera->front = NS_VECTOR3(0.0f, 0.0f, -1.0f);
    camera->up = NS_VECTOR3(0.0f, 1.0f, 0.0f);

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;

    camera->target = nsVector3_zero;
    camera->distance = 30.0f;

    return camera;
}

void nsCamera_free(nsCamera *camera) {
    if (!camera) return;

    NS_FREE(camera);
}

void nsCamera_update(nsCamera *camera) {
    camera->pitch = ns_clamp(camera->pitch, -89.9f, 89.9f);

    float pitch_r = NS_RADIANS(camera->pitch);
    float yaw_r = NS_RADIANS(camera->yaw);
    float pitch_c = ns_cos(pitch_r);
    float pitch_s = ns_sin(pitch_r);
    float yaw_c = ns_cos(yaw_r);
    float yaw_s = ns_sin(yaw_r);

    // spherical -> cartesian
    nsVector3 sphere = nsVector3_zero;
    sphere.x = yaw_c * pitch_c;
    sphere.y = pitch_s;
    sphere.z = yaw_s * pitch_c;

    if (camera->mode == nsCameraMode_FIRST_PERSON) {
        camera->front = nsVector3_normalize(sphere);
        camera->target = nsVector3_add(camera->position, camera->front);
    }
    else if (camera->mode == nsCameraMode_ORBIT) {
        camera->distance = ns_clamp(camera->distance, 0.5f, 100.0f);
        camera->position = nsVector3_add(camera->target, nsVector3_mul(sphere, camera->distance));
    }

    camera->view_mat = nsMatrix4_look_at(camera->position, camera->target, camera->up);
}

void nsCamera_move(nsCamera *camera, float amount) {
    camera->position = nsVector3_add(camera->position, nsVector3_mul(camera->front, amount));
}

void nsCamera_strafe(nsCamera *camera, float amount) {
    nsVector3 right = nsVector3_normalize(nsVector3_cross(camera->front, camera->up));
    camera->position = nsVector3_sub(camera->position, nsVector3_mul(right, amount));
}