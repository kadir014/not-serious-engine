/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/texture.h"


nsTexture *nsTexture_new() {
    nsTexture *texture = NS_NEW(nsTexture);
    NS_MEM_CHECK(texture);

    glGenTextures(1, &texture->texture_id);

    glBindTexture(GL_TEXTURE_2D, texture->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}

void nsTexture_free(nsTexture *texture) {
    if (!texture) return;

    glDeleteTextures(1, &texture->texture_id);

    NS_FREE(texture);
}

void nsTexture_write(nsTexture *texture, size_t width, size_t height, ns_u8 *data) {
    glBindTexture(GL_TEXTURE_2D, texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

int nsTexture_write_from_file(nsTexture *texture, const char *filepath) {
    SDL_Surface *surf = IMG_Load(filepath);

    if (!surf) {
        ns_throw_error(IMG_GetError(), 0, nsErrorSeverity_ERROR);
        return 1;
    }

    nsTexture_write(texture, (size_t)surf->w, (size_t)surf->h, (ns_u8 *)surf->pixels);
    SDL_FreeSurface(surf);

    return 0;
}

int nsTexture_fill(nsTexture *texture, nsColor color) {
    SDL_Surface *surf = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);

    if (!surf) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_ERROR);
        return 1;
    }

    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, (ns_u8)(color.r * 255.0f), (ns_u8)(color.g * 255.0f), (ns_u8)(color.b * 255.0f)));

    nsTexture_write(texture, (size_t)surf->w, (size_t)surf->h, (ns_u8 *)surf->pixels);
    SDL_FreeSurface(surf);

    return 0;
}