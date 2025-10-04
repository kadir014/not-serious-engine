/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_MODEL_H
#define _NS_MODEL_H

#include "engine/include/_internal.h"
#include "engine/include/math/transform.h"
#include "engine/include/graphics/mesh.h"
#include "engine/include/graphics/material.h"


/**
 * @file model/model.h
 * 
 * @brief Model type.
 */


/**
 * @brief Higher level renderable 3D object.
 */
typedef struct {
    nsTransform xform;
    nsMatrix4 xform_mat;
    nsMesh *mesh;
} nsModel;

/**
 * @brief Create new model.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.

 * @return nsModel *
 */
nsModel *nsModel_new(nsMesh *mesh);

/**
 * @brief Free model.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param model Model to free
 */
void nsModel_free(nsModel *model);

void nsModel_set_position(nsModel *model, nsVector3 position);

nsVector3 nsModel_get_position(nsModel *model);

void nsModel_set_euler_angles(nsModel *model, nsVector3 rotation);

nsVector3 nsModel_get_euler_angles(nsModel *model);

void nsModel_set_scale(nsModel *model, nsVector3 scale);

nsVector3 nsModel_get_scale(nsModel *model);

void nsModel_render(nsModel *model);


#endif