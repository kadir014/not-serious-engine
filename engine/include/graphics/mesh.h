/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file graphics/mesh.h
 * @brief Abstract mesh type.
 */
#ifndef _NS_MESH_H
#define _NS_MESH_H

#include "engine/include/_internal.h"
#include "engine/include/core/array.h"
#include "engine/include/graphics/material.h"
#include "engine/include/graphics/buffer.h"


/**
 * @brief Abstract type that manages a collection of buffers and materials.
 */
typedef struct {
    ns_u32 vao_id; /**< GL vertex array object. */

    nsArray *buffers; /**< Array of assigned buffers, first is the primary. */

    nsMaterial *material; /**< Assigned material. */
} nsMesh;

/**
 * @brief Create new mesh.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param material Material
 * @return nsMesh *
 */
nsMesh *nsMesh_new(nsMaterial *material);

/**
 * @brief Free mesh.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param mesh Mesh to free
 */
void nsMesh_free(nsMesh *mesh);

/**
 * @brief Factory function for a mesh initiated with a cube geometry.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param material 
 * @return nsMesh *
 */
nsMesh *nsMesh_from_cube(
    nsMaterial *material,
    float width,
    float height,
    float length,
    float tiling_x,
    float tiling_y
);

nsMesh *nsMesh_from_plane(
    nsMaterial *material,
    float width,
    float length,
    float tiling_x,
    float tiling_y
);

/**
 * @brief Push new buffer to the mesh.
 * 
 * Returns non-zero on error. Use @ref ns_get_error to get more information.
 * 
 * @param mesh Mesh
 * @param buffer New buffer
 * @return int 
 */
int nsMesh_push_buffer(nsMesh *mesh, nsBuffer *buffer);

void nsMesh_initialize(nsMesh *mesh);

void nsMesh_render(nsMesh *mesh);


#endif