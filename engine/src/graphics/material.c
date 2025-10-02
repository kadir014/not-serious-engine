/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/material.h"
#include "engine/include/core/io.h"


/**
 * @file graphics/material.c
 * 
 * @brief Material type.
 */


/**
 * @brief Load shader from string source.
 * 
 * Returns 0 on error, else the shader ID.
 * 
 * @param source Source GLSL code.
 * @param shader_type Shader GL type.
 * @return ns_u32
 */
static ns_u32 load_shader(const char *source, ns_u32 shader_type) {
    ns_u32 shader_id = glCreateShader(shader_type);
    if (!shader_id) {
        ns_throw_error(
            "Shader creation failed.",
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        return 0;
    }
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_buffer[400];
        glGetShaderInfoLog(shader_id, 400, NULL, info_buffer);
        ns_throw_error(
            info_buffer,
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        return 0;
    }

    return shader_id;
}


nsMaterial *nsMaterial_new(
    const char *vertex_shader_source,
    const char *fragment_shader_source
) {
    nsMaterial *material = NS_NEW(nsMaterial);
    NS_MEM_CHECK(material);

    ns_u32 vertex_shader = load_shader(vertex_shader_source, GL_VERTEX_SHADER);
    if (!vertex_shader) return NULL;

    ns_u32 fragment_shader = load_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    material->program = glCreateProgram();
    if (!material->program) {
        ns_throw_error(
            "Shader program creation failed.",
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return NULL;
    }
    glAttachShader(material->program, vertex_shader);
    glAttachShader(material->program, fragment_shader);
    glLinkProgram(material->program);

    int success;
    glGetProgramiv(material->program, GL_LINK_STATUS, &success);
    if(!success) {
        ns_throw_error(
            "Shader program linkage failed.",
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        glDeleteProgram(material->program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return NULL;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return material;
}

nsMaterial *nsMaterial_from_files(
    const char *vertex_shader_source_filepath,  
    const char *fragment_shader_source_filepath
) {
    char *vertex_shader_source = ns_read_file_raw(vertex_shader_source_filepath);
    if (!vertex_shader_source) return NULL;

    char *fragment_shader_source = ns_read_file_raw(fragment_shader_source_filepath);
    if (!fragment_shader_source) {
        NS_FREE(vertex_shader_source);
        return NULL;
    }

    nsMaterial *material = nsMaterial_new(vertex_shader_source, fragment_shader_source);

    NS_FREE(vertex_shader_source);
    NS_FREE(fragment_shader_source);

    return material;
}

void nsMaterial_free(nsMaterial *material) {
    if (!material) return;

    glDeleteProgram(material->program);

    NS_FREE(material);
}