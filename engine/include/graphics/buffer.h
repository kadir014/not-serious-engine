/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_BUFFER_H
#define _NS_BUFFER_H

#include "engine/include/_internal.h"


/**
 * @file graphics/buffer.h
 * 
 * @brief Buffer type.
 */


/**
 * @brief TODO
 */
typedef struct {
    ns_u32 buffer_id; /**< GL buffer object. */
    ns_u32 attribute_loc; /**< Attribute location in the VAO. */
    ns_u32 components; /**< Number of components per element. */
    size_t stride; /**< Byte stride between elements. */
    size_t count; /**< Number of elements. */
} nsBuffer;

/**
 * @brief Create new buffer.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @return nsBuffer *
 */
nsBuffer *nsBuffer_new(ns_u32 attribute_loc, ns_u32 components);

/**
 * @brief Free buffer.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param buffer Buffer to free
 */
void nsBuffer_free(nsBuffer *buffer);

/**
 * @brief Write data on buffer.
 * 
 * @param buffer Buffer
 * @param data Array of data
 * @param count Size of the array
 */
void nsBuffer_write(nsBuffer *buffer, float *data, size_t count);


#endif