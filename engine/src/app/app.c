/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/app/app.h"
#include "engine/include/math/vector.h"
#include "engine/include/math/matrix.h"
#include "engine/include/math/math.h"
#include "engine/include/graphics/material.h"
#include "engine/include/graphics/mesh.h"
#include "engine/include/graphics/buffer.h"
#include "engine/include/graphics/texture.h"
#include "engine/include/model/model.h"
#include "engine/include/loaders/obj.h"
#include "engine/include/core/pool.h"
#include "engine/include/core/profiler.h"
#include "engine/include/scene/camera.h"

#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_sdl_gl3.h"


nsApp *ns_global_app = NULL;


nsApp *nsApp_new(nsAppDefinition app_def) {
    // There can only be one app instance.
    if (ns_global_app) {
        return ns_global_app;
    }

    nsApp *app = NS_NEW(nsApp);
    NS_MEM_CHECK(app);

    app->app_def = app_def;
    app->is_running = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        return NULL;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        ns_throw_error(IMG_GetError(), 0, nsErrorSeverity_FATAL);
        SDL_Quit();
        return NULL;
    }

    //TODO: Request version and multisampling
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    app->window = SDL_CreateWindow(
        app_def.window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app_def.window_width,
        app_def.window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!app->window) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        IMG_Quit();
        SDL_Quit();
        return NULL;
    }

    app->gl_ctx = SDL_GL_CreateContext(app->window);
    if (!app->gl_ctx) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        SDL_DestroyWindow(app->window);
        IMG_Quit();
        SDL_Quit();
        return NULL;
    }
    SDL_GL_MakeCurrent(app->window, app->gl_ctx);

    if (!gladLoaderLoadGL()) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        SDL_GL_DeleteContext(app->gl_ctx);
        SDL_DestroyWindow(app->window);
        IMG_Quit();
        SDL_Quit();
        return NULL;
    }

    SDL_GL_SetSwapInterval(app_def.vsync);

    ns_global_app = app;
    return app;
}

void nsApp_free(nsApp *app) {
    if (!app) return;

    // Free all scenes
    // for scene in app.scenes: scene.on_free()
    if (app->current_scene->on_free) {
        app->current_scene->on_free(app->current_scene);
    }

    nk_sdl_shutdown();
    SDL_GL_DeleteContext(app->gl_ctx);
    SDL_DestroyWindow(app->window);
    IMG_Quit();
    SDL_Quit();

    NS_FREE(app);
}

typedef struct {
    int major;
    int minor;
    int profile;
    char profile_str[16];
} GLVersionInfo;

GLVersionInfo fetch_opengl_version() {
    GLVersionInfo version;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &version.major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &version.minor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &version.profile);

    switch (version.profile) {
        case (SDL_GL_CONTEXT_PROFILE_CORE):
            strcpy(version.profile_str, "Core");
            break;

        case (SDL_GL_CONTEXT_PROFILE_COMPATIBILITY):
            strcpy(version.profile_str, "Compatibility");
            break;

        case (SDL_GL_CONTEXT_PROFILE_ES):
            strcpy(version.profile_str, "ES");
            break;

        default:
            strcpy(version.profile_str, "Unknown");
            break;
    }

    return version;
}

void nsApp_run(nsApp *app) {
    app->ui_ctx = nk_sdl_init(app->window);
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    struct nk_font *font;
    font = nk_font_atlas_add_default(atlas, 14, NULL);
    nk_sdl_font_stash_end();
    nk_style_set_font(app->ui_ctx, &font->handle);

    // Initialize all scenes
    // TODO: resource manager shenanigans...
    // for scene in app.scenes: scene.on_ready()
    if (app->current_scene->on_ready) {
        app->current_scene->on_ready(app->current_scene);
    }

    // Reset all scenes
    // for scene in app.scenes: scene.on_ready()
    if (app->current_scene->on_reset) {
        app->current_scene->on_reset(app->current_scene);
    }

    app->is_running = true;
    while (app->is_running) {
        // TODO: clock tick

        nk_input_begin(app->ui_ctx);
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                nsApp_stop(app);
            }

            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    nsApp_stop(app);
                }
            }

            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = event.window.data1;
                    int height = event.window.data2;
                    glViewport(0, 0, width, height);
                }
            }

            // TODO: inputsystem->process(event)
            nk_sdl_handle_event(&event);
        }
        nk_sdl_handle_grab();
        nk_input_end(app->ui_ctx);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        app->current_scene->on_render(app->current_scene);

        nk_sdl_render(
            NK_ANTI_ALIASING_ON,
            100 * 1024,
            25 * 1024
        );

        SDL_GL_SwapWindow(app->window);
    }
}

void nsApp_stop(nsApp *app) {
    app->is_running = false;
}

void nsApp_push_scene(nsApp *app, nsScene *scene) {
    app->current_scene = scene;
}