/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/loaders/obj.h"
#include "engine/include/core/io.h"
#include "engine/include/core/profiler.h"

static inline ns_bool is_whitespace(char chr) {
    return (chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n');
}

#define ADVANCE obj->current++

static inline void skip_whitespace(nsOBJ *obj) {
    while (*obj->current != '\0' && is_whitespace(*obj->current)) {
        ADVANCE;
    }
}

static inline void skip_line(nsOBJ *obj) {
    while (*obj->current != '\0') {
        ADVANCE;
        if (*obj->current == '\n') {
            break;
        }
    }

    // Skin \n
    ADVANCE;
}

// KOD NE YAPIYO ANLA, KENDIN TEKRAR YAZ, HATTA OPTIMIZE ET?

static inline float fast_atof(const char **p) {
    const char *s = *p;
    float sign = 1.0f;
    if (*s == '-') { sign = -1.0f; s++; }
    else if (*s == '+') { s++; }

    float value = 0.0f;
    while (*s >= '0' && *s <= '9') {
        value = value * 10.0f + (*s - '0');
        s++;
    }

    if (*s == '.') {
        s++;
        float frac = 0.0f;
        float scale = 1.0f;
        while (*s >= '0' && *s <= '9') {
            frac = frac * 10.0f + (*s - '0');
            scale *= 10.0f;
            s++;
        }
        value += frac / scale;
    }

    *p = s;
    return value * sign;
}

static inline int fast_atoi(const char **p) {
    const char *s = *p;
    int sign = 1;
    if (*s == '-') { sign = -1; s++; }

    int value = 0;
    while (*s >= '0' && *s <= '9') {
        value = value * 10 + (*s - '0');
        s++;
    }
    *p = s;
    return value * sign;
}

static inline float parse_float(nsOBJ *obj) {
    // char *end;
    // float value = strtof(obj->current, &end);
    // obj->current = end;
    // return value;

    return fast_atof(&obj->current);
}

static inline long parse_long(nsOBJ *obj) {
    // char *end;
    // long value = strtol(obj->current, &end, 10);
    // obj->current = end;
    // return value;
    return (long)fast_atoi(&obj->current);
}

static inline void parse_vertex(nsOBJ *obj) {
    /*
        Syntax:
        v float float float
    */

    ADVANCE; // skip v

    skip_whitespace(obj);
    float x = parse_float(obj);

    skip_whitespace(obj);
    float y = parse_float(obj);

    skip_whitespace(obj);
    float z = parse_float(obj);

    nsVector3 vertex = NS_VECTOR3(x, y, z);
    nsPool_add(obj->vertices, &vertex);
}

static inline void parse_normal(nsOBJ *obj) {
    /*
        Syntax:
        vn float float float
    */

    ADVANCE; ADVANCE; // skip vn

    skip_whitespace(obj);
    float x = parse_float(obj);

    skip_whitespace(obj);
    float y = parse_float(obj);

    skip_whitespace(obj);
    float z = parse_float(obj);

    nsVector3 normal = NS_VECTOR3(x, y, z);
    nsPool_add(obj->normals, &normal);
}

static inline void parse_uv(nsOBJ *obj) {
    /*
        Syntax:
        vt float float
    */

    ADVANCE; ADVANCE; // skip vt

    skip_whitespace(obj);
    float x = parse_float(obj);

    skip_whitespace(obj);
    float y = parse_float(obj);

    nsVector2 uv = NS_VECTOR2(x, y);
    nsPool_add(obj->uvs, &uv);
}

static inline void parse_face(nsOBJ *obj) {
    /*
        Syntax:
        f int/[int]/[int] int/[int]/[int] int/[int]/[int]
    */

    // TODO: optional normals & uvs

    ADVANCE; // skip f

    skip_whitespace(obj);

    long v0 = parse_long(obj);

    ADVANCE;
    long uv0 = parse_long(obj);

    ADVANCE;
    long n0 = parse_long(obj);

    skip_whitespace(obj);

    long v1 = parse_long(obj);

    ADVANCE;
    long uv1 = parse_long(obj);

    ADVANCE;
    long n1 = parse_long(obj);

    skip_whitespace(obj);

    long v2 = parse_long(obj);

    ADVANCE;
    long uv2 = parse_long(obj);

    ADVANCE;
    long n2 = parse_long(obj);

    nsOBJFace face = {
        .vertex_ids = {v0, v1, v2},
        .normal_ids = {n0, n1, n2},
        .uv_ids = {uv0, uv1, uv2}
    };
    nsPool_add(obj->faces, &face);
}

static void parse_obj(nsOBJ *obj) {
    while (*obj->current != '\0') {
        skip_whitespace(obj);

        if (*obj->current == '#') {
            skip_line(obj);
        }

        else if (*obj->current == 'v' && is_whitespace(*(obj->current + 1))) {
            parse_vertex(obj);
        }

        else if (*obj->current == 'v' && *(obj->current + 1) == 'n') {
            parse_normal(obj);
        }

        else if (*obj->current == 'v' && *(obj->current + 1) == 't') {
            parse_uv(obj);
        }

        else if (*obj->current == 'f') {
            parse_face(obj);
        }
        
        else {
            skip_line(obj);
        }
    }
}


nsOBJ nsOBJ_load_raw(char *source) {
    nsOBJ obj;

    obj.vertices = nsPool_new(sizeof(nsVector3));
    obj.normals = nsPool_new(sizeof(nsVector3));
    obj.uvs = nsPool_new(sizeof(nsVector2));
    obj.faces = nsPool_new(sizeof(nsOBJFace));

    obj.mesh.tris = nsPool_new(sizeof(nsOBJTri));

    obj.current = source;

    parse_obj(&obj);

    for (size_t i = 0; i < obj.faces->size; i++) {
        nsOBJFace *face = nsPool_get(obj.faces, i);

        nsOBJTri tri = {
            .vertices = {
                *(nsVector3 *)nsPool_get(obj.vertices, face->vertex_ids[0] - 1),
                *(nsVector3 *)nsPool_get(obj.vertices, face->vertex_ids[1] - 1),
                *(nsVector3 *)nsPool_get(obj.vertices, face->vertex_ids[2] - 1)
            },
            .normals = {
                *(nsVector3 *)nsPool_get(obj.normals, face->normal_ids[0] - 1),
                *(nsVector3 *)nsPool_get(obj.normals, face->normal_ids[1] - 1),
                *(nsVector3 *)nsPool_get(obj.normals, face->normal_ids[2] - 1)
            },
            .uvs = {
                *(nsVector2 *)nsPool_get(obj.uvs, face->uv_ids[0] - 1),
                *(nsVector2 *)nsPool_get(obj.uvs, face->uv_ids[1] - 1),
                *(nsVector2 *)nsPool_get(obj.uvs, face->uv_ids[2] - 1)
            },
        };

        nsPool_add(obj.mesh.tris, &tri);
    }

    nsPool_free(obj.vertices);
    nsPool_free(obj.normals);
    nsPool_free(obj.uvs);
    nsPool_free(obj.faces);

    return obj;
}

nsOBJ nsOBJ_load(const char *filepath) {
    char *content = ns_read_file_raw(filepath);
    if (!content) {
        return (nsOBJ){0};
    }

    nsOBJ obj = nsOBJ_load_raw(content);

    NS_FREE(content);
    
    return obj;
}