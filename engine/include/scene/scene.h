/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file scene/scene.h
 * @brief Scene.
 */
#ifndef _NS_SCENE_H
#define _NS_SCENE_H

#include "engine/include/_internal.h"


struct _nsScene;

typedef void ( *nsSceneCallback)(struct _nsScene *state);


/**
 * @brief A scene is basically one game state.
 */
struct _nsScene{
    char *name; /**< Name of the scene. */
    nsSceneCallback on_ready; /**< Initialize the scene once. */
    nsSceneCallback on_free; /**< Release all resources used by scene. */
    nsSceneCallback on_reset; /**< Reset runtime state of the scene.  */
    nsSceneCallback on_active; /**< The scene is activated. */
    nsSceneCallback on_deactive; /**< The scene is deactivated. */
    nsSceneCallback on_tick; /**< Fixed-rate tick callback. */
    nsSceneCallback on_render; /**< Render callback. */
};

typedef struct _nsScene nsScene;


#endif