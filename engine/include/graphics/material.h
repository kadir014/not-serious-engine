/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_MATERIAL_H
#define _NS_MATERIAL_H

#include "engine/include/_internal.h"
#include "engine/include/graphics/uniform.h"
#include "engine/include/core/array.h"
#include "engine/include/math/vector.h"
#include "engine/include/math/matrix.h"


/**
 * @file graphics/material.h
 * 
 * @brief Abstract material type.
 */


/**
 * @brief Abstract type that encapsulates a GPU shader program and potential
 * associated data such as textures, uniforms and states.
 */
typedef struct {
    ns_u32 program_id; /**< GL shader program object. */

    nsArray *uniforms_cache;
} nsMaterial;

/**
 * @brief Create new material.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param vertex_shader_source Vertex shader source string
 * @param fragment_shader_source Fragment shader source string
 * @return nsMaterial *
 */
nsMaterial *nsMaterial_new(
    const char *vertex_shader_source,
    const char *fragment_shader_source
);

/**
 * @brief Create new material from shader source files.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param vertex_shader_source_filepath Vertex shader source filepath
 * @param fragment_shader_source_filepath Fragment shader source filepath
 * @return nsMaterial *
 */
nsMaterial *nsMaterial_from_files(
    const char *vertex_shader_source_filepath,  
    const char *fragment_shader_source_filepath
);

/**
 * @brief Free material.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param material Material to free
 */
void nsMaterial_free(nsMaterial *material);

/**
 * @brief Get uniform, cache it if first time.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param material Material
 * @param name Uniform name
 * @return nsUniform *
 */
nsUniform *nsMaterial_get_uniform(nsMaterial *material, char *name);

void nsMaterial_set_uniform_vector3(
    nsMaterial *material,
    char *name,
    nsVector3 vec
);

void nsMaterial_set_uniform_matrix4(
    nsMaterial *material,
    char *name,
    nsMatrix4 mat
);

nsMaterial_set_uniform_float(nsMaterial *material, char *name, float value);

nsMaterial_set_uniform_int(nsMaterial *material, char *name, int value);


#endif