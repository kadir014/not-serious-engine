/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file graphics/texture.h
 * @brief Texture type.
 */
#ifndef _NS_TEXTURE_H
#define _NS_TEXTURE_H

#include "engine/include/_internal.h"
#include "engine/include/graphics/color.h"


/**
 * @brief
 */
typedef struct {
    ns_u32 texture_id; /**< GL texture object. */
} nsTexture;

/**
 * @brief Create new texture.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.

 * @return nsTexture *
 */
nsTexture *nsTexture_new();

/**
 * @brief Free texture.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param texture Texture to free
 */
void nsTexture_free(nsTexture *texture);

void nsTexture_write(nsTexture *texture, size_t width, size_t height, ns_u8 *data);

int nsTexture_write_from_file(nsTexture *texture, const char *filepath);

int nsTexture_fill(nsTexture *texture, nsColor color);


#endif