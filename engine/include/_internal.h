/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_INTERNAL_H
#define _NS_INTERNAL_H


/**
 * @file _internal.h
 * 
 * @brief Internal API header.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/gl.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "engine/include/core/types.h"
#include "engine/include/core/error.h"
#include "engine/include/core/platform.h"
#include "engine/include/core/version.h"


/*
    Tracy profiler & allocation macros.

    NS_MALLOC -> stdlib malloc or Tracy equivalent.
    NS_REALLOC -> stdlib's realloc or Tracy equivalent.
    NS_FREE -> stdlib's free or Tracy equivalent.

    NS_TRACY_ZONE_START -> Start profiled function zone.
    NS_TRACY_ZONE_END -> End profiled function zone.
    NS_TRACY_FRAMEMARK -> New tracy frame. Only once!
*/
#ifdef TRACY_ENABLE

    #include "TracyC.h"

    #define NS_TRACY_ZONE_START TracyCZone(_tracy_zone, true)
    #define NS_TRACY_ZONE_END TracyCZoneEnd(_tracy_zone)
    #define NS_TRACY_FRAMEMARK TracyCFrameMark

    static inline void *NS_MALLOC(size_t size) {
        void *ptr = malloc(size);
        TracyCAlloc(ptr, size);
        return ptr;
    }

    static inline void *NS_REALLOC(void *ptr, size_t new_size) {
        if (ptr) {
            TracyCFree(ptr);
        }

        void *new_ptr = realloc(ptr, new_size);
        TracyCAlloc(new_ptr, new_size);

        return new_ptr;
    }

    static inline void NS_FREE(void *ptr) {
        TracyCFree(ptr);
        free(ptr);
    }

#else

    #define NS_TRACY_ZONE_START
    #define NS_TRACY_ZONE_END
    #define NS_TRACY_FRAMEMARK

    #define NS_MALLOC(size) malloc(size)
    #define NS_REALLOC(ptr, new_size) realloc(ptr, new_size)
    #define NS_FREE(ptr) free(ptr)

#endif


// Allocate new object of given type.
#define NS_NEW(type) ((type *)NS_MALLOC(sizeof(type)))


#endif