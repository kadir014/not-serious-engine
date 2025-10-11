/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/core/error.h"


nsError _ns_global_error = {
    .message = "",
    .code = 0,
    .severity = nsErrorSeverity_INFO
};


nsLogger _ns_global_logger = {
    .outs = {NULL},
    .min_severity = nsErrorSeverity_DEBUG
};


nsError ns_get_error() {
    return _ns_global_error;
}


nsLogger *ns_get_logger() {
    return &_ns_global_logger;
}

void ns_log(const char *message, nsErrorSeverity severity) {
    const char *severity_str = nsErrorSeverity_as_string(severity);

    for (size_t i = 0; i < NS_LOGGER_MAX_OUT_STREAMS; i++) {
        FILE *stream = _ns_global_logger.outs[i];

        if (!stream) continue;

        fprintf(stream, "[%-7s] %s\n", severity_str, message);
    }
}