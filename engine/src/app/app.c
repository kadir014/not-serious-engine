/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/app/app.h"
#include "engine/include/graphics/material.h"
#include "engine/include/math/vector.h"
#include "engine/include/math/matrix.h"
#include "engine/include/math/math.h"


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

    //TODO: Request version and multisampling
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

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
        SDL_Quit();
        return NULL;
    }

    app->gl_ctx = SDL_GL_CreateContext(app->window);
    if (!app->gl_ctx) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return NULL;
    }
    SDL_GL_MakeCurrent(app->window, app->gl_ctx);

    if (!gladLoaderLoadGL()) {
        ns_throw_error(SDL_GetError(), 0, nsErrorSeverity_FATAL);
        SDL_GL_DeleteContext(app->gl_ctx);
        SDL_DestroyWindow(app->window);
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
    SDL_Quit();

    NS_FREE(app);
}

void cube_factory(float vertices[108], float normals[108]) {
    const float cube_vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        // Left face
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right face
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

        // Top face
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f
    };

    const float cube_normals[] = {
        // Front (0,0,1)
         0,0,1,  0,0,1,  0,0,1,
         0,0,1,  0,0,1,  0,0,1,
        // Back (0,0,-1)
         0,0,-1, 0,0,-1, 0,0,-1,
         0,0,-1, 0,0,-1, 0,0,-1,
        // Left (-1,0,0)
        -1,0,0, -1,0,0, -1,0,0,
        -1,0,0, -1,0,0, -1,0,0,
        // Right (1,0,0)
         1,0,0,  1,0,0,  1,0,0,
         1,0,0,  1,0,0,  1,0,0,
        // Top (0,1,0)
         0,1,0,  0,1,0,  0,1,0,
         0,1,0,  0,1,0,  0,1,0,
        // Bottom (0,-1,0)
         0,-1,0, 0,-1,0, 0,-1,0,
         0,-1,0, 0,-1,0, 0,-1,0
    };

    for (size_t i = 0; i < 108; i++) {
        vertices[i] = cube_vertices[i];
    }

    for (size_t i = 0; i < 108; i++) {
        normals[i] = cube_normals[i];
    }
}

void nsApp_run(nsApp *app) {
    app->is_running = true;

    nsMaterial *material = nsMaterial_from_files(
        "../game/src/shaders/base.vsh",
        "../game/src/shaders/base.fsh"
    );
    if (!material) {
        printf(ns_get_error().message);
        return;
    }
    printf("Shaders loaded successfully! (id: %u)\n", material->program);

    float vertices[108];
    float normals[108];
    size_t vertex_count = 36;
    cube_factory(vertices, normals);

    ns_u32 in_position_location = 0;
    ns_u32 VBO;
    glGenBuffers(1, &VBO);

    ns_u32 in_normal_location = 1;
    ns_u32 NBO;
    glGenBuffers(1, &NBO);

    ns_u32 VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(in_position_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(in_position_location);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(in_normal_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(in_normal_location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    nsMatrix4 model = nsMatrix4_identity;
    // model.m[5] = ns_cos(NS_RADIANS(-55.0f));
    // model.m[6] = ns_sin(NS_RADIANS(-55.0f));
    // model.m[9] = -ns_sin(NS_RADIANS(-55.0f));
    // model.m[10] = ns_cos(NS_RADIANS(-55.0f));

    nsMatrix4 view = nsMatrix4_identity;
    view = nsMatrix4_translate(view, NS_VECTOR3(0.0f, 0.0f, -3.0f));

    nsMatrix4 projection = nsMatrix4_perspective(NS_RADIANS(45.0f), 1280.0f/720.0f, 0.1f, 1000.0f);

    // Before changing uniforms bind the program!
    glUseProgram(material->program);

    ns_u32 u_model = glGetUniformLocation(material->program, "u_model");
    glUniformMatrix4fv(u_model, 1, GL_FALSE, model.m);

    ns_u32 u_view = glGetUniformLocation(material->program, "u_view");
    glUniformMatrix4fv(u_view, 1, GL_FALSE, view.m);

    ns_u32 u_projection = glGetUniformLocation(material->program, "u_projection");
    glUniformMatrix4fv(u_projection, 1, GL_FALSE, projection.m);

    nsVector3 camera_pos = NS_VECTOR3(0.0f, 0.0f, 13.0f);
    nsVector3 camera_front = NS_VECTOR3(0.0f, 0.0f, -1.0f);
    nsVector3 camera_up = NS_VECTOR3(0.0f, 1.0f, 0.0f);
    float camera_yaw = -90.0;
    float camera_pitch = 0.0;

    float camera_speed = 0.1f;
    float camera_horizontal_sensitivity = 0.1f;
    float camera_vertical_sensitivity = 0.1f;

    SDL_SetRelativeMouseMode(true);

    float a = 0.0;

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

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            a += 1.0;
            model.m[5] = ns_cos(NS_RADIANS(a));
            model.m[6] = ns_sin(NS_RADIANS(a));
            model.m[9] = -ns_sin(NS_RADIANS(a));
            model.m[10] = ns_cos(NS_RADIANS(a));
            glUniformMatrix4fv(u_model, 1, GL_FALSE, model.m);

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
            glUniformMatrix4fv(u_view, 1, GL_FALSE, view.m);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

            glUseProgram(material->program);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
        }

        SDL_GL_SwapWindow(app->window);
    }
}

void nsApp_stop(nsApp *app) {
    app->is_running = false;
}