/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/core/array.h"


/**
 * @file core/array.c
 * 
 * @brief Type-generic dynamically growing array implementation.
 */


nsArray *nsArray_new() {
    nsArray *array = NS_NEW(nsArray);
    NS_MEM_CHECK(array);

    array->size = 0;
    array->max = 1;
    array->growth_factor = 2.0;
    array->data = (void **)NS_MALLOC(sizeof(void *));
    if (!array->data) NS_FREE(array);
    NS_MEM_CHECK(array->data);

    return array;
}

nsArray *nsArray_new_ex(size_t default_capacity, float growth_factor) {
    nsArray *array = NS_NEW(nsArray);
    NS_MEM_CHECK(array);

    array->size = 0;
    array->max = default_capacity;
    array->growth_factor = growth_factor;
    array->data = (void **)NS_MALLOC(sizeof(void *) * default_capacity);
    if (!array->data) NS_FREE(array);
    NS_MEM_CHECK(array->data);

    return array;
}

void nsArray_free(nsArray *array) {
    if (!array) return;

    NS_FREE(array->data);
    NS_FREE(array);
}

void nsArray_free_each(nsArray *array, nsArray_free_each_callback free_func) {
    for (size_t i = 0; i < array->size; i++)
        free_func(array->data[i]);
}

int nsArray_add(nsArray *array, void *elem) {
    // Only reallocate when max capacity is reached
    if (array->size == array->max) {
        array->size++;
        array->max = (size_t)((float)array->max * array->growth_factor);
        array->data = NS_REALLOC(array->data, array->max * sizeof(void *));
        NS_MEM_CHECK_I(array->data);
    }
    else {
        array->size++;
    }

    array->data[array->size - 1] = elem;

    return 0;
}

void *nsArray_pop(nsArray *array, size_t index) {
    for (size_t i = 0; i < array->size; i++) {
        if (i == index) {
            array->size--;
            void *elem = array->data[i];

            array->data[i] = array->data[array->size];
            array->data[array->size] = NULL;

            return elem;
        }
    }

    return NULL;
}

size_t nsArray_remove(nsArray *array, void *elem) {
    for (size_t i = 0; i < array->size; i++) {
        if (array->data[i] == elem) {
            array->size--;

            array->data[i] = array->data[array->size];
            array->data[array->size] = NULL;

            return i;
        }
    }

    return -1;
}

int nsArray_clear(nsArray *array, void (free_func)(void *)) {
    /*
        We can set array->max to 0 and reallocate but
        not doing it might be more efficient for the developer
        since they will probably fill the array up again.
        Maybe a separate parameter for this?
    */

    if (array->size == 0) return 0;
   
    if (!free_func) {
        while (array->size > 0) {
            if (!nsArray_pop(array, 0))
                return 1;
        }
    }

    else {
        while (array->size > 0) {
            void *p = nsArray_pop(array, 0);
            if (!p) return 1;
            free_func(p);
        }
    }

    return 0;
}

size_t nsArray_total_memory_used(nsArray *array) {
    size_t array_s = sizeof(nsArray);
    array_s += array->max * sizeof(intptr_t);
    return array_s;
}