/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file graphics/color.h
 * @brief Color type.
 */
#ifndef _NS_COLOR_H
#define _NS_COLOR_H

#include "engine/include/_internal.h"


/**
 * @brief Color definition in normalized RGB space [0, 1].
 */
typedef struct {
    float r; /**< Red channel of the color. */
    float g; /**< Green channel of the color. */
    float b; /**< Blue channel of the color. */
    float a; /**< Alpha channel of the color. */
} nsColor;

#define NS_RGB(r, g, b) ((nsColor){(r), (g), (b), 1.0f})

#define NS_RGBA(r, g, b, a) ((nsColor){(r), (g), (b), (a)})


#endif