/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_ARRAY_H
#define _NS_ARRAY_H

#include "engine/include/_internal.h"


/**
 * @file core/array.h
 * 
 * @brief Type-generic dynamically growing array implementation.
 */


/**
 * @brief Type-generic dynamically growing array implementation.
 */
typedef struct {
    size_t size; /**< Length of the array. */
    size_t max; /**< Maximum size the array ever reached, this is basically the size of the array on HEAP. */
    float growth_factor; /**< Scaling factor for reallocations. */
    void **data; /**< Array of void pointers. */
} nsArray;

/**
 * @brief Create new array.
 * 
 * @return nsArray *
 */
nsArray *nsArray_new();

/**
 * @brief Create new array with more control than @ref nsArray_new
 * 
 * @param default_capacity Default allocation size at initialization
 * @param growth_factor Scaling factor for reallocations
 * @return nsArray *
 */
nsArray *nsArray_new_ex(size_t default_capacity, float growth_factor);

/**
 * @brief Free array.
 * 
 * It's safe to pass `NULL` to this function.
 * 
 * @param array Array to free
 */
void nsArray_free(nsArray *array);

typedef void (*nsArray_free_each_callback)(void *);

/**
 * @brief Free each element of array.
 * 
 * @param array Array
 * @param free_func Free function
 */
void nsArray_free_each(nsArray *array, nsArray_free_each_callback free_func);

/**
 * @brief Add new element to array.
 * 
 * Returns non-zero on error. Use @ref ns_get_error to get more information.
 * 
 * @param array Array to append to
 * @param elem Void pointer to element
 * @return int Status
 */
int nsArray_add(nsArray *array, void *elem);

/**
 * @brief Remove element by index from array and return the element. Returns `NULL` if failed.
 * 
 * @note The array is not sorted after removal, meaning the array gets slightly randomized every remove call.
 * 
 * @param array Array
 * @param index Index of element to remove
 * @return void *
 */
void *nsArray_pop(nsArray *array, size_t index);

/**
 * @brief Remove element from array and return the index. Returns `-1` if failed.
 * 
 * @note The array is not sorted after removal, meaning the array gets slightly randomized every remove call.
 * 
 * @param array Array
 * @param elem Element to remove
 * @return size_t
 */
size_t nsArray_remove(nsArray *array, void *elem);

/**
 * @brief Clear the array.
 * 
 * Elements are not freed if `NULL` is passed as freeing function. Use @ref ns_get_error to get more information.
 * 
 * Returns non-zero on error.
 * 
 * @param array Array
 * @param free_func Free function
 * @return int Status
 */
int nsArray_clear(nsArray *array, void (free_func)(void *));

/**
 * @brief Get the total amount of memory used by this array instance.
 * 
 * Elements are counted as pointers.
 * 
 * @param array Array
 * @return size_t 
 */
size_t nsArray_total_memory_used(nsArray *array);


#endif