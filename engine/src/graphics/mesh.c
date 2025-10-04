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

    glDeleteVertexArrays(1, &mesh->vao_id);

    nsArray_free_each(mesh->buffers, (nsArray_free_each_callback)nsBuffer_free);
    nsArray_free(mesh->buffers);

    NS_FREE(mesh);
}

void nsMesh_push_buffer(nsMesh *mesh, nsBuffer *buffer) {
    nsArray_add(mesh->buffers, buffer);
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
    glUseProgram(mesh->material->program_id);

    // TODO: Make this option better
    nsBuffer *primary_buffer = mesh->buffers->data[0];
    size_t vertex_count = primary_buffer->count;

    glBindVertexArray(mesh->vao_id);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glBindVertexArray(0);
}