/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file core/error.h
 * @brief Basic error handling & logging.
 */
#ifndef _NS_ERROR_H
#define _NS_ERROR_H

#include <stdio.h>


/**
 * @brief Severity levels for error and log messages.
 */
typedef enum {
    nsErrorSeverity_DEBUG,   /**< Verbose message used for debugging and diagnostics. */
    nsErrorSeverity_INFO,    /**< General informational message. */
    nsErrorSeverity_WARNING, /**< An unexpected situation occurred,
                                  but execution can safely continue with minimal impact. */
    nsErrorSeverity_ERROR,   /**< A significant problem occurred;
                                  part of the system may not function correctly,
                                  but execution can continue in a degraded state. */
    nsErrorSeverity_FATAL    /**< A critical failure occurred;
                                  the system cannot continue execution and must shut down. */
} nsErrorSeverity;

/**
 * @brief Severity level as string.
 * 
 * @param severity Severity level
 * @return const char *
 */
static inline const char *nsErrorSeverity_as_string(nsErrorSeverity severity) {
    switch (severity) {
        case nsErrorSeverity_DEBUG:
            return "DEBUG";

        case nsErrorSeverity_INFO:
            return "INFO";

        case nsErrorSeverity_WARNING:
            return "WARNING";

        case nsErrorSeverity_ERROR:
            return "ERROR";

        case nsErrorSeverity_FATAL:
            return "FATAL";

        default:
            return "UNKNOWN";
    }
}

/**
 * @brief Common error codes used commonly in the engine.
 */
typedef enum {
    nsErrorCode_DEFAULT = 0,
    nsErrorCode_ALLOCATION_FAILED = 1,
    nsErrorCode_SHADER_COMPILATION_FAILED = 2
} nsErrorCode;


#define NS_ERROR_BUFFER_SIZE 512

/**
 * @brief Error type.
 */
typedef struct {
    char message[NS_ERROR_BUFFER_SIZE]; /**< Error message. */
    nsErrorCode code; /**< Error code. */
    nsErrorSeverity severity; /**< Error severity level. */
} nsError;

/**
 * @brief Global error state. 
 * 
 * TODO: Error stack
 */
extern nsError _ns_global_error;


#define NS_LOGGER_MAX_OUT_STREAMS 8

/**
 * @brief Logging system for recording messages with different severity levels.
 * 
 * The logger can output to multiple streams (e.g., stdout, stderr, log files).
 * Each message is filtered based on the minimum severity level specified,
 * allowing only messages equal to or above that level to be written.
 */
typedef struct {
    FILE *outs[NS_LOGGER_MAX_OUT_STREAMS]; /**< Output streams used by the logger (e.g., stdout, files). */
    nsErrorSeverity min_severity; /**< Minimum severity level required for a message to be logged. */
} nsLogger;

/**
 * @brief Global logger instance.
 */
extern nsLogger _ns_global_logger;


/**
 * @brief Throw error and log.
 * 
 * This HAS to be a macro to get __FILE__ and __LINE__.
 * 
 * @param message Error message
 * @param code Error code
 * @param severity Error severity
 */
#define ns_throw_error(message_, code_, severity_) {             \
    _ns_global_error.code = (nsErrorCode)code_;                  \
    _ns_global_error.severity = (nsErrorSeverity)severity_;      \
    sprintf(                                                     \
        _ns_global_error.message,                                \
        "Error in %s, line %d: %s",                              \
        __FILE__, __LINE__, message_                             \
    );                                                           \
    ns_log(_ns_global_error.message, _ns_global_error.severity); \
}

/**
 * @brief Get the last occured error.
 * 
 * @return nsError
 */
nsError ns_get_error();


/**
 * @brief Check allocated memory, return `NULL` & throw error if necessary.
 * 
 * @param object Allocated space
 */
#define NS_MEM_CHECK(object) {             \
    if (!(object)) {                       \
        ns_throw_error(                    \
            "Failed to allocate memory.",  \
            nsErrorCode_ALLOCATION_FAILED, \
            nsErrorSeverity_FATAL);        \
        return NULL;                       \
    }                                      \
}                                          \

/**
 * @brief Check allocated memory, return 1 & throw error if necessary.
 * 
 * @param object Allocated space
 */
#define NS_MEM_CHECK_I(object) {           \
    if (!(object)) {                       \
        ns_throw_error(                    \
            "Failed to allocate memory.",  \
            nsErrorCode_ALLOCATION_FAILED, \
            nsErrorSeverity_FATAL          \
        );                                 \
        return 1;                          \
    }                                      \
}                                          \

/**
 * @brief Get the reference to global logger.
 * 
 * @return nsLogger *
 */
nsLogger *ns_get_logger();

/**
 * @brief Log message.
 * 
 * @param message Message
 * @param severity Severity level
 */
void ns_log(const char *message, nsErrorSeverity severity);


#endif