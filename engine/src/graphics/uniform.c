/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/uniform.h"


/**
 * @file graphics/uniform.c
 * 
 * @brief Uniform storage buffer.
 */


nsUniform *nsUniform_new(char *name, ns_u32 location) {
    nsUniform *uniform = NS_NEW(nsUniform);
    NS_MEM_CHECK(uniform);

    uniform->name = name;
    uniform->location = location;

    return uniform;
}

void nsUniform_free(nsUniform *uniform) {
    if (!uniform) return;

    NS_FREE(uniform);
}