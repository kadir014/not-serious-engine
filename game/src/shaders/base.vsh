/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#version 460

in vec3 in_position;
in vec3 in_normal;
in vec2 in_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_normal;
out vec3 v_frag_pos;
out vec2 v_uv;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);

    v_normal = mat3(transpose(inverse(u_model))) * in_normal;
    v_frag_pos = vec3(u_model * vec4(in_position, 1.0));
    v_uv = in_uv;
}