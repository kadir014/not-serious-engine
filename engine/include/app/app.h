#ifndef _NS_APP_H
#define _NS_APP_H

#include "engine/include/_internal.h"


/**
 * @file app/app.h
 * 
 * @brief Top-level application context.
 */


typedef struct {
    const char *window_title;
    ns_u32 window_width;
    ns_u32 window_height;
    ns_u32 target_fps;
    ns_bool vsync;
} nsAppDefinition;


/**
 * @brief Top-level application context that manages subsystems.
 */
typedef struct {
    nsAppDefinition app_def;
    ns_bool is_running;

    SDL_Window *window;
    SDL_GLContext *gl_ctx;
} nsApp;

/**
 * @brief Create & initialize new application.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param app_def 
 * @return nsApp * 
 */
nsApp *nsApp_new(nsAppDefinition app_def);

/**
 * @brief Free application and all the resources it manages.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param app 
 */
void nsApp_free(nsApp *app);

void nsApp_run(nsApp *app);

void nsApp_stop(nsApp *app);


#endif