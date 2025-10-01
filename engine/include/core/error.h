#ifndef _NS_ERROR_H
#define _NS_ERROR_H

#include <stdio.h>


/**
 * @file core/error.h
 * 
 * @brief Basic error handling & logging.
 */


/**
 * @brief Error severity variations.
 */
typedef enum {
    nsErrorSeverity_INFO,
    nsErrorSeverity_WARNING,
    nsErrorSeverity_ERROR,
    nsErrorSeverity_FATAL
} nsErrorSeverity;

/**
 * @brief Common error codes used commonly in the engine.
 */
typedef enum {
    nsErrorCode_ALLOCATION_FAILED = 1,
} nsErrorCode;


/*
    Global error state.
    TODO: Do I need an error stack?
*/

#define NS_ERROR_BUFFER_SIZE 512

typedef struct {
    char message[NS_ERROR_BUFFER_SIZE];
    unsigned int code;
    unsigned int severity;
} nsError;

extern nsError _ns_global_error;


/**
 * @brief Throw error and replace the global error state.
 * 
 * @param message Error message
 * @param code Error code
 * @param severity Error severity
 */
#define ns_throw_error(message_, code_, severity_) {                            \
    _ns_global_error.code = (unsigned int)code_;                              \
    _ns_global_error.severity = (unsigned int)severity_;                      \
    char *_severity_str;                                                     \
    if (severity_ == nsErrorSeverity_INFO) _severity_str = "INFO";            \
    else if (severity_ == nsErrorSeverity_WARNING) _severity_str = "WARNING"; \
    else if (severity_ == nsErrorSeverity_ERROR) _severity_str = "ERROR";     \
    else if (severity_ == nsErrorSeverity_FATAL) _severity_str = "FATAL";     \
    sprintf(                                                                 \
        _ns_global_error.message,                                            \
        "[%s] code %d in %s, line %d: %s\n",                                 \
        _severity_str, code_, __FILE__, __LINE__, message_                     \
    );                                                                       \
}

/**
 * @brief Get the last occured error.
 * 
 * @return char *
 */
nsError ns_get_error();


/**
 * @brief Check allocated memory, return `NULL` & raise error if necessary.
 * 
 * @param object Allocated space
 */
#define NS_MEM_CHECK(object) {              \
    if (!(object)) {                        \
        ns_set_error(                       \
            "Failed to allocate memory.",   \
            nsErrorCode_ALLOCATION_FAILED,  \
            nsErrorSeverity_FATAL);         \
        return NULL;                        \
    }                                       \
}                                           \

/**
 * @brief Check allocated memory, return 1 & raise error if necessary.
 * 
 * @param object Allocated space
 */
#define NS_MEM_CHECK_I(object) {           \
    if (!(object)) {                       \
        ns_set_error(                      \
            "Failed to allocate memory.",  \
            nsErrorCode_ALLOCATION_FAILED, \
            nsErrorSeverity_FATAL          \
        );                                 \
        return 1;                          \
    }                                      \
}                                          \


#endif