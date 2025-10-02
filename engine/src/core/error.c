/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/core/error.h"


/**
 * @file core/error.c
 * 
 * @brief Basic error handling & logging.
 */


nsError _ns_global_error = {
    .message = "",
    .code = 0,
    .severity = nsErrorSeverity_INFO
};


nsError ns_get_error() {
    return _ns_global_error;
}