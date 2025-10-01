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