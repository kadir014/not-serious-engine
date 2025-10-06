/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/mesh.h"


/**
 * @file graphics/mesh.c
 * 
 * @brief Abstract mesh type.
 */


nsMesh *nsMesh_new(nsMaterial *material) {
    nsMesh *mesh = NS_NEW(nsMesh);
    NS_MEM_CHECK(mesh);

    mesh->material = material;

    mesh->buffers = nsArray_new();
    if (!mesh->buffers) {
        NS_FREE(mesh);
        return NULL;
    }

    glGenVertexArrays(1, &mesh->vao_id);
    // TODO: check vao id

    return mesh;
}

void nsMesh_free(nsMesh *mesh) {
    if (!mesh) return;

    nsArray_free_each(mesh->buffers, (nsArray_free_each_callback)nsBuffer_free);
    nsArray_free(mesh->buffers);

    nsMaterial_free(mesh->material);

    glDeleteVertexArrays(1, &mesh->vao_id);

    NS_FREE(mesh);
}

nsMesh *nsMesh_from_cube(
    nsMaterial *material,
    float width,
    float height,
    float length,
    float tiling_x,
    float tiling_y
) {
    float width_h = width * 0.5f;
    float height_h = height * 0.5f;
    float length_h = length * 0.5f;

    float vertices[108] = {
        -width_h, -height_h,  length_h,
         width_h, -height_h,  length_h,
         width_h,  height_h,  length_h,
        -width_h, -height_h,  length_h,
         width_h,  height_h,  length_h,
        -width_h,  height_h,  length_h,

        -width_h, -height_h, -length_h,
        -width_h,  height_h, -length_h,
         width_h,  height_h, -length_h,
        -width_h, -height_h, -length_h,
         width_h,  height_h, -length_h,
         width_h, -height_h, -length_h,

        -width_h, -height_h, -length_h,
        -width_h, -height_h,  length_h,
        -width_h,  height_h,  length_h,
        -width_h, -height_h, -length_h,
        -width_h,  height_h,  length_h,
        -width_h,  height_h, -length_h,

         width_h, -height_h, -length_h,
         width_h,  height_h, -length_h,
         width_h,  height_h,  length_h,
         width_h, -height_h, -length_h,
         width_h,  height_h,  length_h,
         width_h, -height_h,  length_h,

        -width_h,  height_h, -length_h,
        -width_h,  height_h,  length_h,
         width_h,  height_h,  length_h,
        -width_h,  height_h, -length_h,
         width_h,  height_h,  length_h,
         width_h,  height_h, -length_h,

        -width_h, -height_h, -length_h,
         width_h, -height_h, -length_h,
         width_h, -height_h,  length_h,
        -width_h, -height_h, -length_h,
         width_h, -height_h,  length_h,
        -width_h, -height_h,  length_h
    };

    float normals[108] = {
         0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

         0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,
         0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,

        -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

         1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

         0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

         0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f
    };

    float uvs[72] = {
        0.0f,     0.0f,
        tiling_x, 0.0f,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        0.0f,     tiling_y,

        0.0f,     0.0f,
        0.0f,     tiling_y,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        tiling_x, 0.0f,

        0.0f,     0.0f,
        tiling_x, 0.0f,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        0.0f,     tiling_y,

        0.0f,     0.0f,
        0.0f,     tiling_y,
        tiling_x, tiling_y,
        0.0f,      0.0f,
        tiling_x, tiling_y,
        tiling_x, 0.0f,

        0.0f,     0.0f,
        0.0f,     tiling_y,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        tiling_x, 0.0f,

        0.0f,     0.0f,
        tiling_x, 0.0f,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        0.0f,     tiling_y
    };

    nsBuffer *vertices_buffer = nsBuffer_new(0, 3);
    nsBuffer_write(vertices_buffer, vertices, 36);

    nsBuffer *normals_buffer = nsBuffer_new(1, 3);
    nsBuffer_write(normals_buffer, normals, 36);

    nsBuffer *uvs_buffer = nsBuffer_new(2, 2);
    nsBuffer_write(uvs_buffer, uvs, 36);

    nsMesh *mesh = nsMesh_new(material);
    nsMesh_push_buffer(mesh, vertices_buffer);
    nsMesh_push_buffer(mesh, normals_buffer);
    nsMesh_push_buffer(mesh, uvs_buffer);
    nsMesh_initialize(mesh);

    return mesh;
}

nsMesh *nsMesh_from_plane(
    nsMaterial *material,
    float width,
    float length,
    float tiling_x,
    float tiling_y
) {
    float width_h = width * 0.5f;
    float length_h = length * 0.5f;

    float vertices[18] = {
        -width_h,  0.0f, -length_h,
        -width_h,  0.0f,  length_h,
         width_h,  0.0f,  length_h,
        -width_h,  0.0f, -length_h,
         width_h,  0.0f,  length_h,
         width_h,  0.0f, -length_h,
    };

    float normals[18] = {
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    };

    float uvs[12] = {
        0.0f,     0.0f,
        0.0f,     tiling_y,
        tiling_x, tiling_y,
        0.0f,     0.0f,
        tiling_x, tiling_y,
        tiling_x, 0.0f,
    };

    nsBuffer *vertices_buffer = nsBuffer_new(0, 3);
    nsBuffer_write(vertices_buffer, vertices, 6);

    nsBuffer *normals_buffer = nsBuffer_new(1, 3);
    nsBuffer_write(normals_buffer, normals, 6);

    nsBuffer *uvs_buffer = nsBuffer_new(2, 2);
    nsBuffer_write(uvs_buffer, uvs, 6);

    nsMesh *mesh = nsMesh_new(material);
    nsMesh_push_buffer(mesh, vertices_buffer);
    nsMesh_push_buffer(mesh, normals_buffer);
    nsMesh_push_buffer(mesh, uvs_buffer);
    nsMesh_initialize(mesh);

    return mesh;
}

int nsMesh_push_buffer(nsMesh *mesh, nsBuffer *buffer) {
    return nsArray_add(mesh->buffers, buffer);
}

void nsMesh_initialize(nsMesh *mesh) {
    glBindVertexArray(mesh->vao_id);
    
    for (size_t i = 0; i < mesh->buffers->size; i++) {
        nsBuffer *buffer = mesh->buffers->data[i];

        glBindBuffer(GL_ARRAY_BUFFER, buffer->buffer_id);
        glVertexAttribPointer(
            buffer->attribute_loc,
            buffer->components,
            GL_FLOAT,
            GL_FALSE,
            buffer->stride,
            (void *)0
        );
        glEnableVertexAttribArray(buffer->attribute_loc);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void nsMesh_render(nsMesh *mesh) {
    if (mesh->material) {
        glUseProgram(mesh->material->program_id);
    }

    // TODO: Make this option better
    nsBuffer *primary_buffer = mesh->buffers->data[0];
    size_t vertex_count = primary_buffer->count;

    glBindVertexArray(mesh->vao_id);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glBindVertexArray(0);
}