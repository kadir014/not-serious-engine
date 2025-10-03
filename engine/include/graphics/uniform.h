/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_UNIFORM_H
#define _NS_UNIFORM_H

#include "engine/include/_internal.h"


/**
 * @file graphics/uniform.h
 * 
 * @brief Uniform storage buffer type.
 */


/**
 * @brief Uniform storage buffer type.
 */
typedef struct {
    char *name; /**< Name of the uniform. */
    ns_i32 location; /**< Location of the uniform in the shader program object. */
} nsUniform;

/**
 * @brief Create new uniform.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param name Name of the uniform.
 * @param location Location of the uniform in the shader program object.
 * @return nsUniform *
 */
nsUniform *nsUniform_new(char *name, ns_u32 location);

/**
 * @brief Free uniform.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param uniform Uniform to free
 */
void nsUniform_free(nsUniform *uniform);


#endif