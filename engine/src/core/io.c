/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/_internal.h"
#include "engine/include/core/io.h"


/**
 * @file core/io.c
 * 
 * @brief Basic I/O operations.
 */


char *ns_read_file_raw(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        ns_throw_error("Failed to open file.", 0, nsErrorSeverity_ERROR);
        return NULL;
    }

    // Seek to the end & rewind back to determine the file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = NS_MALLOC(length + 1);
    if (!buffer) {
        fclose(file);
        NS_MEM_CHECK(buffer);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}