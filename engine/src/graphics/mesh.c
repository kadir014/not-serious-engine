/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/mesh.h"


/**
 * @file graphics/material.c
 * 
 * @brief Material type.
 */


nsMesh *nsMesh_new(nsMaterial *material) {
    nsMesh *mesh = NS_NEW(nsMesh);
    NS_MEM_CHECK(mesh);

    mesh->material = material;

    glGenVertexArrays(1, &mesh->vao);

    mesh->vertex_count = 0;
    mesh->normal_count = 0;
    mesh->uv_count = 0;

    return mesh;
}

void nsMesh_free(nsMesh *mesh) {
    if (!mesh) return;

    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vertex_bo);
    glDeleteBuffers(1, &mesh->normal_bo);
    glDeleteBuffers(1, &mesh->uv_bo);

    NS_FREE(mesh);
}

void nsMesh_render(nsMesh *mesh) {
    glUseProgram(mesh->material->program);

    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
    glBindVertexArray(0);
}