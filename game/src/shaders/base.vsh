/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#version 460

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_normal;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);

    v_normal = mat3(transpose(inverse(u_model))) * in_normal;
}