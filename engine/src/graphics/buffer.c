/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/buffer.h"


/**
 * @file graphics/buffer.c
 * 
 * @brief Buffer type.
 */


nsBuffer *nsBuffer_new(ns_u32 attribute_loc, ns_u32 components) {
    nsBuffer *buffer = NS_NEW(nsBuffer);
    NS_MEM_CHECK(buffer);

    glGenBuffers(1, &buffer->buffer_id);
    // TODO: buffer_id creation fail check

    buffer->attribute_loc = attribute_loc;
    buffer->components = components;

    buffer->stride = sizeof(float) * buffer->components; // TODO: internal
    buffer->count = 0;

    return buffer;
}

void nsBuffer_free(nsBuffer *buffer) {
    if (!buffer) return;

    glDeleteBuffers(1, &buffer->buffer_id);

    NS_FREE(buffer);
}

void nsBuffer_write(nsBuffer *buffer, float *data, size_t count) {
    buffer->count = count;
    glBindBuffer(GL_ARRAY_BUFFER, buffer->buffer_id);
    // TODO: static draw, dynamic draw, diger buffer data fonksiyonu, vs...
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count * buffer->components, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}