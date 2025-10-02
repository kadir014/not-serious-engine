/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_IO_H
#define _NS_IO_H

#include <stdio.h>


/**
 * @file core/io.h
 * 
 * @brief Basic I/O operations.
 */


/**
 * @brief Read file in binary mode.
 * 
 * This returns an allocated string, caller has to manage memory.
 * 
 * Returns `NULL` on error. Use @ref ns_get_error to get more information.
 * 
 * @param filepath Filepath
 * @return char *
 */
char *ns_read_file_raw(const char *filepath);


#endif