/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "game/src/scenes/materialdemo.h"


static nsTexture *diffuse_map;
static nsTexture *specular_map;
static struct nk_colorf diffuse_color;
static struct nk_colorf specular_color;
static struct nk_colorf dirlight_color;
static float specular = 1.0;
static int colored_specular = 1;

static nsMaterial *material;
static nsModel *model;
static nsCamera *camera;


static void on_ready(nsScene *scene) {
    material  = nsMaterial_from_files(
        "../game/src/shaders/base.vsh",
        "../game/src/shaders/phong.fsh"
    );

    nsOBJ obj = nsOBJ_load("../game/assets/models/shaderball.obj");
    model = nsModel_new(nsMesh_from_obj(material, &obj));
    nsModel_set_position(model, NS_VECTOR3(0.0f, -6.0f, 0.0f));

    diffuse_map = nsTexture_new();
    specular_map = nsTexture_new();

    float aspect = (float)ns_global_app->app_def.window_width / (float)ns_global_app->app_def.window_height;
    camera = nsCamera_new(nsCameraProjection_PERSPECTIVE, aspect);
    nsMaterial_set_uniform_matrix4(material, "u_projection", camera->projection_mat);
}

static void on_free(nsScene *scene) {
    nsModel_free(model);
    nsTexture_free(diffuse_map);
    nsTexture_free(specular_map);
    nsCamera_free(camera);
}

static void on_reset(nsScene *scene) {
    nsMaterial_set_uniform_vector3(material, "directional_light.direction", NS_VECTOR3(-3.5f, -3.0f, 1.0f));
    nsMaterial_set_uniform_vector3(material, "directional_light.color", NS_VECTOR3(1.0f, 1.0f, 1.0f));
    nsMaterial_set_uniform_float(material, "directional_light.ambient_intensity", 0.1f);
    nsMaterial_set_uniform_int(material, "point_lights_count", 0);
    nsMaterial_set_uniform_float(material, "material.shininess", 32.0);

    dirlight_color = (struct nk_colorf){1.0f, 1.0f, 1.0f, 1.0f};
    diffuse_color = (struct nk_colorf){1.0f, 1.0f, 1.0f, 1.0f};
    specular_color = (struct nk_colorf){0.05f, 0.05f, 0.05f, 1.0f};
    nsMaterial_set_uniform_float(material, "material.shininess", 5.95f);

    nsTexture_fill(specular_map, NS_RGB(diffuse_color.r, diffuse_color.g, diffuse_color.b));
    nsTexture_fill(specular_map, NS_RGB(specular_color.r, specular_color.g, specular_color.b));

    camera->yaw = 36.0f;
    camera->pitch = 25.0f;
}

static void on_render(nsScene *scene) {
    struct nk_context *ui_ctx = ns_global_app->ui_ctx;

    // UI
    {
        if (nk_begin(ui_ctx, "Material Editor", nk_rect(0.0f, 0.0f, 300.0f, 430.0f), NK_WINDOW_TITLE | NK_WINDOW_MOVABLE)) {
            char display_buf[16];
            const float ratio0[3] = {0.30f, 0.40f, 0.30f};
            const float ratio1[3] = {0.34f, 0.46f, 0.20f};

            if (nk_tree_push(ui_ctx, NK_TREE_TAB, "Phong Properties", NK_MAXIMIZED)) {
                {
                    nk_layout_row(ui_ctx, NK_DYNAMIC, 45, 3, ratio0);

                    nk_label(ui_ctx, "Diffuse", NK_TEXT_LEFT);

                    diffuse_color = nk_color_picker(ui_ctx, diffuse_color, NK_RGB);
                    nsTexture_fill(diffuse_map, NS_RGB(diffuse_color.r, diffuse_color.g, diffuse_color.b));

                    sprintf(display_buf, "%1.1f,%1.1f,%1.1f", diffuse_color.r, diffuse_color.g, diffuse_color.b);
                    nk_label(ui_ctx, display_buf, NK_TEXT_LEFT);
                }

                nk_layout_row_dynamic(ui_ctx, 18, 1);
                nk_checkbox_label(ui_ctx, "Colored specular", &colored_specular);

                if (colored_specular) {
                    nk_layout_row(ui_ctx, NK_DYNAMIC, 45, 3, ratio0);

                    nk_label(ui_ctx, "Specular", NK_TEXT_LEFT);

                    specular_color = nk_color_picker(ui_ctx, specular_color, NK_RGB);
                    nsTexture_fill(specular_map, NS_RGB(specular_color.r, specular_color.g, specular_color.b));

                    sprintf(display_buf, "%1.1f,%1.1f,%1.1f", specular_color.r, specular_color.g, specular_color.b);
                    nk_label(ui_ctx, display_buf, NK_TEXT_LEFT);
                }
                else {
                    nk_layout_row(ui_ctx, NK_DYNAMIC, 18, 3, ratio1);

                    nk_label(ui_ctx, "Specular", NK_TEXT_LEFT);

                    if (nk_slider_float(ui_ctx, 0.0f, &specular, 1.0f, 0.05f)) {
                        nsTexture_fill(specular_map, NS_RGB(specular, specular, specular));
                    }
                    
                    sprintf(display_buf, "%3.2f", specular);
                    nk_label(ui_ctx, display_buf, NK_TEXT_LEFT);
                }

                {
                    nk_layout_row(ui_ctx, NK_DYNAMIC, 18, 3, ratio1);

                    nk_label(ui_ctx, "Shininess", NK_TEXT_LEFT);

                    float value = nsMaterial_get_uniform_float(material, "material.shininess");
                    nk_slider_float(ui_ctx, 1.0f, &value, 100.0f, 0.05f);
                    
                    sprintf(display_buf, "%3.2f", value);
                    nk_label(ui_ctx, display_buf, NK_TEXT_LEFT);

                    nsMaterial_set_uniform_float(material, "material.shininess", value);
                }

                nk_layout_row_dynamic(ui_ctx, 18, 1);
                nk_label(ui_ctx, "Premade Materials:", NK_TEXT_LEFT);
                if (nk_button_label(ui_ctx, "White plastic")) {
                    colored_specular = 1;
                    diffuse_color = (struct nk_colorf){1.0f, 1.0f, 1.0f, 1.0f};
                    specular_color = (struct nk_colorf){0.05f, 0.05f, 0.05f, 1.0f};
                    nsMaterial_set_uniform_float(material, "material.shininess", 5.95f);
                }
                if (nk_button_label(ui_ctx, "Gold")) {
                    colored_specular = 1;
                    diffuse_color = (struct nk_colorf){0.960f, 0.847f, 0.113f, 1.0f};
                    specular_color = (struct nk_colorf){1.0f, 0.780f, 0.490f, 1.0f};
                    nsMaterial_set_uniform_float(material, "material.shininess", 60.0f);
                }
                if (nk_button_label(ui_ctx, "Chrome")) {
                    colored_specular = 1;
                    diffuse_color = (struct nk_colorf){0.35f, 0.35f, 0.35f, 1.0f};
                    specular_color = (struct nk_colorf){0.517f, 0.560f, 0.7f, 1.0f};
                    nsMaterial_set_uniform_float(material, "material.shininess", 12.8f);
                }
                if (nk_button_label(ui_ctx, "Ruby")) {
                    colored_specular = 1;
                    diffuse_color = (struct nk_colorf){0.61424f, 0.04136f, 0.04136f, 1.0f};
                    specular_color = (struct nk_colorf){0.727811f, 0.626959f, 0.626959f, 1.0f};
                    nsMaterial_set_uniform_float(material, "material.shininess", 85.0f);
                }

                nk_layout_row_dynamic(ui_ctx, 8, 1);
                nk_spacer(ui_ctx);
                nk_tree_pop(ui_ctx);
            }

            if (nk_tree_push(ui_ctx, NK_TREE_TAB, "Directional Light Properties", NK_MAXIMIZED)) {
                {
                    nk_layout_row(ui_ctx, NK_DYNAMIC, 45, 3, ratio0);

                    nk_label(ui_ctx, "Color", NK_TEXT_LEFT);

                    dirlight_color = nk_color_picker(ui_ctx, dirlight_color, NK_RGB);
                    
                    nsMaterial_set_uniform_vector3(material, "directional_light.color", NS_VECTOR3(dirlight_color.r, dirlight_color.g, dirlight_color.b));

                    sprintf(display_buf, "%1.1f,%1.1f,%1.1f", dirlight_color.r, dirlight_color.g, dirlight_color.b);
                    nk_label(ui_ctx, display_buf, NK_TEXT_LEFT);
                }

                nk_layout_row_dynamic(ui_ctx, 8, 1);
                nk_spacer(ui_ctx);
                nk_tree_pop(ui_ctx);
            }
        }
        nk_end(ui_ctx);
    }


    nsCamera_update(camera);
    nsMaterial_set_uniform_matrix4(material, "u_view", camera->view_mat);
    nsMaterial_set_uniform_vector3(material, "u_view_pos", camera->position);


    nsMaterial_set_uniform_int(material, "material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_map->texture_id);

    nsMaterial_set_uniform_int(material, "material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_map->texture_id);

    nsMaterial_set_uniform_vector3(material, "material.emissive", NS_VECTOR3(0.0f, 0.0f, 0.0f));
    
    nsModel_render(model);
}


nsScene nsMaterialDemoScene = {
    .name = "material_demo",
    .on_ready = on_ready,
    .on_free = on_free,
    .on_reset = on_reset,
    .on_active = NULL,
    .on_deactive = NULL,
    .on_tick = NULL,
    .on_render = on_render
};