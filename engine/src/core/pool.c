/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/core/pool.h"


nsPool *nsPool_new(size_t elem_size) {
    nsPool *pool = NS_NEW(nsPool);
    NS_MEM_CHECK(pool);

    pool->elem_size = elem_size;
    pool->size = 0;
    pool->max = 1;
    pool->growth_factor = 2.0;
    pool->data = (void *)NS_MALLOC(elem_size);
    if (!pool->data) NS_FREE(pool);
    NS_MEM_CHECK(pool->data);

    return pool;
}

nsPool *nsPool_new_ex(
    size_t elem_size,
    size_t default_capacity,
    float growth_factor
) {
    nsPool *pool = NS_NEW(nsPool);
    NS_MEM_CHECK(pool);

    pool->elem_size = elem_size;
    pool->size = 0;
    pool->max = default_capacity;
    pool->growth_factor = growth_factor;
    pool->data = (void *)NS_MALLOC(default_capacity * elem_size);
    if (!pool->data) NS_FREE(pool);
    NS_MEM_CHECK(pool->data);

    return pool;
}

void nsPool_free(nsPool *pool) {
    if (!pool) return;

    NS_FREE(pool->data);
    NS_FREE(pool);
}

int nsPool_add(nsPool *pool, const void *elem) {
    // Only reallocate when max capacity is reached
    if (pool->size >= pool->max) {
        size_t new_capacity = (size_t)((float)pool->max * pool->growth_factor);
        void *new_data = NS_REALLOC(pool->data, new_capacity * pool->elem_size);
        NS_MEM_CHECK_I(new_data);

        pool->data = new_data;
        pool->max = new_capacity;
    }

    memcpy(
        (char *)pool->data + pool->size * pool->elem_size,
        elem,
        pool->elem_size
    );

    pool->size++;

    return 0;
}

void *nsPool_get(nsPool *pool, size_t index) {
    if (index >= pool->size) {
        ns_throw_error("Index is out of bounds.\n", 0, nsErrorSeverity_ERROR);
        return NULL;
    }

    return (char *)pool->data + index * pool->elem_size;
}

int nsPool_remove(nsPool *pool, size_t index);

void nsPool_clear(nsPool *pool);