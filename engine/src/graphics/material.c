/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/graphics/material.h"
#include "engine/include/core/io.h"


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
    ns_u32 vertex_shader = load_shader(vertex_shader_source, GL_VERTEX_SHADER);
    if (!vertex_shader) {
        return NULL;
    }

    ns_u32 fragment_shader = load_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return NULL;
    }

    nsMaterial *material = NS_NEW(nsMaterial);
    if (!material) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        NS_MEM_CHECK(material);
    }

    material->uniforms_cache = nsArray_new();
    if (!material->uniforms_cache) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        NS_FREE(material);
        return NULL;
    }

    material->program_id = glCreateProgram();
    if (!material->program_id) {
        ns_throw_error(
            "Shader program creation failed.",
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        nsArray_free(material->uniforms_cache);
        NS_FREE(material);
        return NULL;
    }
    glAttachShader(material->program_id, vertex_shader);
    glAttachShader(material->program_id, fragment_shader);
    glLinkProgram(material->program_id);

    int success;
    glGetProgramiv(material->program_id, GL_LINK_STATUS, &success);
    if(!success) {
        ns_throw_error(
            "Shader program linkage failed.",
            nsErrorCode_SHADER_COMPILATION_FAILED,
            nsErrorSeverity_FATAL
        );
        glDeleteProgram(material->program_id);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        nsArray_free(material->uniforms_cache);
        NS_FREE(material);
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

    glDeleteProgram(material->program_id);
    
    nsArray_free_each(material->uniforms_cache, (nsArray_free_each_callback)nsUniform_free);
    nsArray_free(material->uniforms_cache);

    NS_FREE(material);
}

nsUniform *nsMaterial_get_uniform(nsMaterial *material, char *name) {
    // Search cache
    for (size_t i = 0; i < material->uniforms_cache->size; i++) {
        nsUniform *uniform = material->uniforms_cache->data[i];
        if (strcmp(uniform->name, name) == 0) {
            return uniform;
        }
    }

    // Not found, query it
    ns_i32 location = glGetUniformLocation(material->program_id, name);
    if (location == -1) {
        ns_throw_error("Uniform not found in shader program.", 0, nsErrorSeverity_WARNING);
        return NULL;
    }

    // Cache it
    nsUniform *uniform = nsUniform_new(name, location);
    nsArray_add(material->uniforms_cache, uniform);
    return uniform;
}

void nsMaterial_set_uniform_vector3(
    nsMaterial *material,
    char *name,
    nsVector3 vec
) {
    nsUniform *uniform = nsMaterial_get_uniform(material, name);

    if (uniform) {
        glUseProgram(material->program_id);
        glUniform3f(uniform->location, vec.x, vec.y, vec.z);
    }
}

void nsMaterial_set_uniform_matrix4(
    nsMaterial *material,
    char *name,
    nsMatrix4 mat
) {
    nsUniform *uniform = nsMaterial_get_uniform(material, name);

    if (uniform) {
        glUseProgram(material->program_id);
        glUniformMatrix4fv(uniform->location, 1, GL_FALSE, mat.m);
    }
}

nsMaterial_set_uniform_float(nsMaterial *material, char *name, float value) {
    nsUniform *uniform = nsMaterial_get_uniform(material, name);

    if (uniform) {
        glUseProgram(material->program_id);
        glUniform1f(uniform->location, value);
    }
}

nsMaterial_set_uniform_int(nsMaterial *material, char *name, int value) {
    nsUniform *uniform = nsMaterial_get_uniform(material, name);

    if (uniform) {
        glUseProgram(material->program_id);
        glUniform1i(uniform->location, value);
    }
}