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


/**
 * @file app/app.h
 * 
 * @brief Top-level application context.
 */


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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

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

    nsMaterial_set_uniform_vector3(material, "point_lights[0].position", NS_VECTOR3(1.0f, 1.0f, 7.0f));
    nsMaterial_set_uniform_vector3(material, "point_lights[0].color", NS_VECTOR3(1.0f, 0.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "point_lights[0].ambient_intensity", 0.1f);

    nsMaterial_set_uniform_vector3(material, "point_lights[1].position", NS_VECTOR3(1.0f, 1.0f, -7.0f));
    nsMaterial_set_uniform_vector3(material, "point_lights[1].color", NS_VECTOR3(1.0f, 1.0f, 0.0f));
    nsMaterial_set_uniform_float(material, "point_lights[1].ambient_intensity", 0.1f);

    nsMaterial_set_uniform_vector3(material, "point_lights[2].position", NS_VECTOR3(1.0f, 1.0f, -7.0f));
    nsMaterial_set_uniform_vector3(material, "point_lights[2].color", NS_VECTOR3(0.0f, 1.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "point_lights[2].ambient_intensity", 0.1f);

    nsMaterial_set_uniform_vector3(material, "point_lights[3].position", NS_VECTOR3(0.0f, 3.0f, 0.0f));
    nsMaterial_set_uniform_vector3(material, "point_lights[3].color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "point_lights[3].ambient_intensity", 0.1f);

    nsMaterial_set_uniform_int(material, "point_lights_count", 4);

    nsTexture *tex = nsTexture_new();
    nsTexture_write_from_file(tex, "../game/assets/textures/uv_debug.png");

    nsModel *light0 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light0, NS_VECTOR3(1.0f, 1.0f, 7.0f));
    nsModel *light1 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light1, NS_VECTOR3(1.0f, 1.0f, -7.0f));
    nsModel *light2 = nsModel_new(nsMesh_from_cube(material, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f));
    nsModel_set_position(light2, NS_VECTOR3(1.0f, 1.0f, -7.0f));

    nsModel *model0 = nsModel_new(nsMesh_from_cube(material, 1.0, 1.0, 1.0, 1.0f, 1.0f));

    nsModel *ground = nsModel_new(nsMesh_from_plane(material, 1.0, 1.0, 1.0f, 1.0f));
    nsModel_set_scale(ground, NS_VECTOR3(1500.0, 1.0, 1500.0));
    nsModel_set_position(ground, NS_VECTOR3(0.0, -4.0, 0.0));

    nsMatrix4 view = nsMatrix4_identity;
    view = nsMatrix4_translate(view, NS_VECTOR3(0.0f, 0.0f, -3.0f));
    nsMaterial_set_uniform_matrix4(material, "u_view", view);

    float aspect = (float)app->app_def.window_width / (float)app->app_def.window_height;
    nsMatrix4 projection = nsMatrix4_perspective(NS_RADIANS(45.0f), aspect, 0.1f, 1000.0f);
    nsMaterial_set_uniform_matrix4(material, "u_projection", projection);

    nsVector3 camera_pos = NS_VECTOR3(0.0f, 0.0f, 13.0f);
    nsVector3 camera_front = NS_VECTOR3(0.0f, 0.0f, -1.0f);
    nsVector3 camera_up = NS_VECTOR3(0.0f, 1.0f, 0.0f);
    float camera_yaw = -90.0;
    float camera_pitch = 0.0;

    float camera_speed = 0.1f;
    float camera_horizontal_sensitivity = 0.1f;
    float camera_vertical_sensitivity = 0.1f;

    SDL_SetRelativeMouseMode(true);

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
    while (app->is_running) {
        // TODO: clock tick
        
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
        }

        {
            nsVector3 rotation = nsModel_get_euler_angles(model0);
            rotation.x += 0.01f;
            rotation.y += 0.02f;
            rotation.z += 0.04f;
            nsModel_set_euler_angles(model0, rotation);
            

            int mouse_rx, mouse_ry;
            SDL_GetRelativeMouseState(&mouse_rx, &mouse_ry);

            camera_yaw += (float)mouse_rx * camera_horizontal_sensitivity;
            camera_pitch += -(float)mouse_ry * camera_vertical_sensitivity;

            if (camera_pitch > 89.9f) camera_pitch = 89.9f;
            else if (camera_pitch < -89.9f) camera_pitch = -89.9f;

            nsVector3 dir = nsVector3_zero;
            float c = ns_cos(NS_RADIANS(camera_pitch));
            dir.x = ns_cos(NS_RADIANS(camera_yaw)) * c;
            dir.y = ns_sin(NS_RADIANS(camera_pitch));
            dir.z = ns_sin(NS_RADIANS(camera_yaw)) * c;
            camera_front = nsVector3_normalize(dir);

            const Uint8 *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_W]) {
                camera_pos = nsVector3_add(camera_pos, nsVector3_mul(camera_front, camera_speed));
            }
            if (keystate[SDL_SCANCODE_S]) {
                camera_pos = nsVector3_sub(camera_pos, nsVector3_mul(camera_front, camera_speed));
            }
            if (keystate[SDL_SCANCODE_A]) {
                nsVector3 right = nsVector3_normalize(nsVector3_cross(camera_front, camera_up));
                camera_pos = nsVector3_sub(camera_pos, nsVector3_mul(right, camera_speed));
            }
            if (keystate[SDL_SCANCODE_D]) {
                nsVector3 right = nsVector3_normalize(nsVector3_cross(camera_front, camera_up));
                camera_pos = nsVector3_add(camera_pos, nsVector3_mul(right, camera_speed));
            }

            view = nsMatrix4_look_at(camera_pos, nsVector3_add(camera_pos, camera_front), camera_up);
            nsMaterial_set_uniform_matrix4(material, "u_view", view);
            nsMaterial_set_uniform_vector3(material, "u_view_pos", camera_pos);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            nsMaterial_set_uniform_vector3(material, "material.ambient", NS_VECTOR3(1.0f, 1.0f, 1.0f));
            nsMaterial_set_uniform_vector3(material, "material.diffuse", NS_VECTOR3(1.0f, 1.0f, 1.0f));
            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_vector3(material, "material.specular", NS_VECTOR3(0.5f, 0.5f, 0.5f));
            nsMaterial_set_uniform_float(material, "material.shininess", 64.0f);
            
            nsModel_render(model0);
            nsModel_render(ground);

            nsMaterial_set_uniform_vector3(material, "material.ambient", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_vector3(material, "material.diffuse", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 1.0f, 1.0f));
            nsMaterial_set_uniform_vector3(material, "material.specular", NS_VECTOR3(0.0f, 0.0f, 0.0f));
            nsMaterial_set_uniform_float(material, "material.shininess", 1.0f);


            a += 0.01f;
            nsVector3 lightpos0 = NS_VECTOR3(ns_cos(a) * 3.0, ns_cos(a) * 3.0, ns_sin(a) * 3.0);
            nsVector3 lightpos1 = NS_VECTOR3(ns_cos(-a) * 3.0, ns_sin(a) * 3.0, ns_sin(-a) * 3.0);
            nsVector3 lightpos2 = NS_VECTOR3(ns_sin(a) * 3.0, ns_sin(-a + 0.3) * 3.0, ns_cos(-a) * 3.0);

            nsModel_set_position(light0, lightpos0);
            nsMaterial_set_uniform_vector3(material, "point_lights[0].position", lightpos0);

            nsModel_set_position(light1, lightpos1);
            nsMaterial_set_uniform_vector3(material, "point_lights[1].position", lightpos1);

            nsModel_set_position(light2, lightpos2);
            nsMaterial_set_uniform_vector3(material, "point_lights[2].position", lightpos2);


            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 0.0f, 1.0f));
            nsModel_render(light0);

            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(1.0f, 1.0f, 0.0f));
            nsModel_render(light1);

            nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(0.0f, 1.0f, 1.0f));
            nsModel_render(light2);
        }

        SDL_GL_SwapWindow(app->window);
    }

    nsModel_free(model0);
}

void nsApp_stop(nsApp *app) {
    app->is_running = false;
}