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

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_sdl_gl3.h"


nsApp *nsApp_new(nsAppDefinition app_def) {
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

    return app;
}

void nsApp_free(nsApp *app) {
    if (!app) return;

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
    app->is_running = true;

    nsMaterial *material = nsMaterial_from_files(
        "../game/src/shaders/base.vsh",
        "../game/src/shaders/phong.fsh"
    );
    if (!material) {
        printf(ns_get_error().message);
        return;
    }

    struct nk_context *ctx = nk_sdl_init(app->window);
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    struct nk_font *font;
    font = nk_font_atlas_add_default(atlas, 14, NULL);
    nk_sdl_font_stash_end();
    nk_style_set_font(ctx, &font->handle);

    nsMaterial_set_uniform_vector3(material, "directional_light.direction", NS_VECTOR3(0.0f, -1.0f, -1.0f));
    nsMaterial_set_uniform_vector3(material, "directional_light.color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "directional_light.ambient_intensity", 0.1f);

    nsMaterial_set_uniform_vector3(material, "point_lights[0].position", NS_VECTOR3(10.0f, 10.0f, 10.0f));
    nsMaterial_set_uniform_vector3(material, "point_lights[0].color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "point_lights[0].ambient_intensity", 0.1f);

    // nsMaterial_set_uniform_vector3(material, "point_lights[1].position", NS_VECTOR3(1.0f, 1.0f, -7.0f));
    // nsMaterial_set_uniform_vector3(material, "point_lights[1].color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    // nsMaterial_set_uniform_float(material, "point_lights[1].ambient_intensity", 0.1f);

    // nsMaterial_set_uniform_vector3(material, "point_lights[2].position", NS_VECTOR3(1.0f, 1.0f, -7.0f));
    // nsMaterial_set_uniform_vector3(material, "point_lights[2].color", NS_VECTOR3(0.0f, 1.0f, 1.0f));
    // nsMaterial_set_uniform_float(material, "point_lights[2].ambient_intensity", 0.1f);

    // nsMaterial_set_uniform_vector3(material, "point_lights[3].position", NS_VECTOR3(0.0f, 3.0f, 0.0f));
    // nsMaterial_set_uniform_vector3(material, "point_lights[3].color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    // nsMaterial_set_uniform_float(material, "point_lights[3].ambient_intensity", 0.1f);

    nsPrecisionTimer timer;
    nsPrecisionTimer_start(&timer);
    nsOBJ obj = nsOBJ_load("../game/assets/models/shaderball.obj");
    double elapsed = nsPrecisionTimer_stop(&timer);
    printf("OBJ loaded in: %f ms\n", elapsed * 1000.0);
    printf("Triangles: %zu\n", obj.mesh.tris->size);
    size_t bytes = nsPool_total_memory_used(obj.mesh.tris);
    double mb = (double)bytes / 1024.0 / 1024.0;
    printf("Memory: %.1f MB (%zu bytes)\n", mb, bytes);

    nsMaterial_set_uniform_int(material, "point_lights_count", 0);

    nsTexture *tex = nsTexture_new();
    //nsTexture_write_from_file(tex, "../game/assets/textures/diamond_ore.png");
    nsTexture_fill(tex, NS_RGB(1.0f, 0.0f, 0.0f));

    struct nk_colorf diffuse_color = {1.0f, 1.0f, 1.0f, 1.0f};

    nsTexture *specular_map = nsTexture_new();
    //nsTexture_write_from_file(specular_map, "../game/assets/textures/diamond_ore_specular.png");
    nsTexture_fill(specular_map, NS_RGB(1.0f, 1.0f, 1.0f));
    float specular = 1.0f;

    nsModel *light0 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light0, NS_VECTOR3(1.0f, 1.0f, 7.0f));
    nsModel *light1 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light1, NS_VECTOR3(1.0f, 1.0f, -7.0f));
    nsModel *light2 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light2, NS_VECTOR3(1.0f, 1.0f, -7.0f));

    //nsModel *model0 = nsModel_new(nsMesh_from_cube(material, 1.0, 1.0, 1.0, 1.0f, 1.0f));
    nsMesh *mesh = nsMesh_from_obj(material, &obj);
    nsModel *model0 = nsModel_new(mesh);
    nsModel_set_position(model0, NS_VECTOR3(0.0f, -6.0f, 0.0f));

    nsModel *ground = nsModel_new(nsMesh_from_plane(material, 1.0, 1.0, 100.0f, 100.0f));
    nsModel_set_scale(ground, NS_VECTOR3(1500.0, 1.0, 1500.0));
    nsModel_set_position(ground, NS_VECTOR3(0.0, -4.0, 0.0));

    float aspect = (float)app->app_def.window_width / (float)app->app_def.window_height;
    nsCamera *camera = nsCamera_new(nsCameraProjection_PERSPECTIVE, aspect);

    nsMaterial_set_uniform_matrix4(material, "u_projection", camera->projection_mat);

    float camera_horizontal_sensitivity = 0.5f;
    float camera_vertical_sensitivity = 0.5f;
    float camera_speed = 0.5f;

    //SDL_SetRelativeMouseMode(true);

    GLVersionInfo glversion = fetch_opengl_version();
    printf(
        "Not-Serious-Engine %d.%d.%d\n"
        "========================\n"
        "Compiler:  %s %s\n"
        "Platform:  %s\n"
        "Arch:      %s\n"
        "SDL:       %d.%d.%d\n"
        "SDL_image: %d.%d.%d\n"
        "OpenGL:    %d.%d %s\n",
        NS_ENGINE_VERSION_MAJOR, NS_ENGINE_VERSION_MINOR, NS_ENGINE_VERSION_PATCH,
        NS_COMPILER_as_string(), NS_COMPILER_VERSION_STR,
        NS_PLATFORM_as_string(),
        NS_ARCH_as_string(),
        SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
        SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL,
        glversion.major, glversion.minor, glversion.profile_str
    );
    float a = 0.0f;
    ns_bool mouse_pressed = false;
    while (app->is_running) {
        // TODO: clock tick
        
        nk_input_begin(ctx);
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                nsApp_stop(app);
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == 3) mouse_pressed = true;
            }

            else if (event.type == SDL_MOUSEBUTTONUP) {
                mouse_pressed = false;
            }

            else if (event.type == SDL_MOUSEWHEEL) {
                camera->distance -= event.wheel.preciseY * 3.0f;
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
        nk_input_end(ctx);

        // UI
        {
            if (nk_begin(ctx, "Settings", nk_rect(0.0f, 0.0f, 300.0f, 300.0f), NK_WINDOW_TITLE | NK_WINDOW_MOVABLE)) {
                char display_buf[16];
                const float ratio[] = {0.40f, 0.47f, 0.13f};

                {
                    nk_layout_row(ctx, NK_DYNAMIC, 32, 2, ratio);

                    nk_label(ctx, "Diffuse", NK_TEXT_LEFT);

                    diffuse_color = nk_color_picker(ctx, diffuse_color, NK_RGB);

                    nsTexture_fill(tex, NS_RGB(diffuse_color.r, diffuse_color.g, diffuse_color.b));
                }

                {
                    nk_layout_row(ctx, NK_DYNAMIC, 16, 3, ratio);

                    nk_label(ctx, "Specular", NK_TEXT_LEFT);

                    if (nk_slider_float(ctx, 0.0f, &specular, 1.0f, 0.05f)) {
                        nsTexture_fill(specular_map, NS_RGB(specular, specular, specular));
                    }
                    
                    sprintf(display_buf, "%3.2f", specular);
                    nk_label(ctx, display_buf, NK_TEXT_LEFT);
                }

                {
                    nk_layout_row(ctx, NK_DYNAMIC, 16, 3, ratio);

                    nk_label(ctx, "Shininess", NK_TEXT_LEFT);

                    float value = nsMaterial_get_uniform_float(material, "material.shininess");
                    nk_slider_float(ctx, 1.0f, &value, 128.0f, 0.05f);
                    
                    sprintf(display_buf, "%3.2f", value);
                    nk_label(ctx, display_buf, NK_TEXT_LEFT);

                    nsMaterial_set_uniform_float(material, "material.shininess", value);
                }
            }
            nk_end(ctx);
        }

        // RENDER
        {
            int mouse_rx, mouse_ry;
            SDL_GetRelativeMouseState(&mouse_rx, &mouse_ry);

            if (mouse_pressed) {
                camera->yaw += (float)mouse_rx * camera_horizontal_sensitivity;
                camera->pitch += (float)mouse_ry * camera_vertical_sensitivity;
            }

            const Uint8 *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_W]) {
                nsCamera_move(camera, camera_speed);
            }
            if (keystate[SDL_SCANCODE_S]) {
                nsCamera_move(camera, -camera_speed);
            }
            if (keystate[SDL_SCANCODE_A]) {
                nsCamera_strafe(camera, camera_speed);
            }
            if (keystate[SDL_SCANCODE_D]) {
                nsCamera_strafe(camera, -camera_speed);
            }
            if (keystate[SDL_SCANCODE_Q]) {
                camera->position = nsVector3_add(camera->position, nsVector3_mul(camera->up, camera_speed));
            }
            if (keystate[SDL_SCANCODE_E]) {
                camera->position = nsVector3_add(camera->position, nsVector3_mul(camera->up, -camera_speed));
            }
            
            nsCamera_update(camera);
            nsMaterial_set_uniform_matrix4(material, "u_view", camera->view_mat);
            nsMaterial_set_uniform_vector3(material, "u_view_pos", camera->position);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            
            nsMaterial_set_uniform_int(material, "material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex->texture_id);  
            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_int(material, "material.specular", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular_map->texture_id);  
            //nsMaterial_set_uniform_float(material, "material.shininess", 64.0f);
            
            nsModel_render(model0);
            //nsModel_render(ground);

            //nsMaterial_set_uniform_vector3(material, "material.diffuse", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 1.0f, 1.0f));
            //nsMaterial_set_uniform_vector3(material, "material.specular", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            //nsMaterial_set_uniform_float(material, "material.shininess", 1.0f);


            a =0.71f;
            nsVector3 lightpos0 = NS_VECTOR3(ns_cos(a) * 3.0, ns_cos(a) * 3.0, ns_sin(a) * 3.0);
            nsVector3 lightpos1 = NS_VECTOR3(ns_cos(-a) * 3.0, ns_sin(a) * 3.0, ns_sin(-a) * 3.0);
            nsVector3 lightpos2 = NS_VECTOR3(ns_sin(a) * 3.0, ns_sin(-a + 0.3) * 3.0, ns_cos(-a) * 3.0);

            lightpos0 = NS_VECTOR3(10.0f, 5.0f, 10.0f);

            nsModel_set_position(light0, lightpos0);
            nsMaterial_set_uniform_vector3(material, "point_lights[0].position", lightpos0);

            //nsModel_set_position(light1, lightpos1);
            //nsMaterial_set_uniform_vector3(material, "point_lights[1].position", lightpos1);

            //nsModel_set_position(light2, lightpos2);
            //nsMaterial_set_uniform_vector3(material, "point_lights[2].position", lightpos2);


            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 1.0f, 1.0f));
            nsModel_render(light0);

            // nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 1.0f, 0.0f));
            // nsModel_render(light1);

            //nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(0.0f, 1.0f, 1.0f));
            //nsModel_render(light2);
        }

        nk_sdl_render(
            NK_ANTI_ALIASING_ON,
            100 * 1024,
            25 * 1024
        );

        SDL_GL_SwapWindow(app->window);
    }

    nsModel_free(model0);

    nk_sdl_shutdown();
}

void nsApp_stop(nsApp *app) {
    app->is_running = false;
}