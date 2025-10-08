/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file core/pool.h
 * @brief Fixed-size type-generic dynamically growing value pool.
 */
#ifndef _NS_POOL_H
#define _NS_POOL_H

#include "engine/include/_internal.h"


/**
 * @brief Fixed-size type-generic dynamically growing value pool.
 * 
 * A dynamic container that stores element *values* directly in contiguous memory.
 * It is designed for small, short-lived objects or plain data structures.
 * 
 * All elements can be type-generic, but they have to share a fixed size. 
 */
typedef struct {
    size_t elem_size; /**< Fixed element size. */
    size_t size; /**< Current length of the pool. */
    size_t max; /**< Maximum size the pool ever reached, this is basically the size on HEAP. */
    float growth_factor; /**< Scaling factor for reallocations. */
    void* data; /**< Contiguous element memory. */
} nsPool;

// TODO: pool function docs

nsPool *nsPool_new(size_t elem_size);

nsPool *nsPool_new_ex(
    size_t elem_size,
    size_t default_capacity,
    float growth_factor
);

void nsPool_free(nsPool *pool);

int nsPool_add(nsPool *pool, const void *elem);

/**
 * @brief Get the reference to an element at index.
 * 
 * This function checks for out of bounds.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param pool 
 * @param index 
 * @return void* 
 */
void *nsPool_get(nsPool *pool, size_t index);

int nsPool_remove(nsPool *pool, size_t index);

void nsPool_clear(nsPool *pool);


#endif