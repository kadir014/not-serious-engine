/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

/**
 * @file loaders/obj.h
 * @brief Wavefront OBJ loader.
 * 
 * OBJ (.obj) files are the simplest type to store geometry and its properties.
 * Follows the specification in https://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
#ifndef _NS_WAVEFRONT_OBJ_H
#define _NS_WAVEFRONT_OBJ_H

#include "engine/include/_internal.h"
#include "engine/include/math/vector.h"
#include "engine/include/core/pool.h"


/**
 * @brief Face definition with property IDs in OBJ file.
 * 
 * f v/n/uv ...
 */
typedef struct {
    long vertex_ids[3];
    long normal_ids[3];
    long uv_ids[3];
} nsOBJFace;

/**
 * @brief Simple triangle face.
 */
typedef struct {
    nsVector3 vertices[3];
    nsVector3 normals[3];
    nsVector2 uvs[3];
} nsOBJTri;

/**
 * @brief Triangular mesh geometry defined in OBJ file.
 */
typedef struct {
    nsPool *tris;
} nsOBJMesh;

/**
 * @brief Type representing loaded Wavefront OBJ file.
 */
typedef struct {
    /*
        Temporary state used by the loader.
    */
    char *current;
    nsPool *vertices;
    nsPool *normals;
    nsPool *uvs;
    nsPool *faces;
    
    /*
        Public members.
    */
    nsOBJMesh mesh;
} nsOBJ;

/**
 * @brief Load OBJ from source null-terminated string.
 * 
 * @param source OBJ content
 * @return nsOBJ 
 */
nsOBJ nsOBJ_load_raw(char *source);

/**
 * @brief Load OBJ from file.
 * 
 * @param filepath 
 * @return nsOBJ 
 */
nsOBJ nsOBJ_load(const char *filepath);


#endif