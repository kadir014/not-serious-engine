/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_MATERIAL_H
#define _NS_MATERIAL_H

#include "engine/include/_internal.h"


/**
 * @file graphics/material.h
 * 
 * @brief Material type.
 */


/**
 * @brief Type that encapsulates a GPU shader program and potential associated
 * data such as textures, uniforms and states.
 */
typedef struct {
    ns_u32 program;
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


#endif