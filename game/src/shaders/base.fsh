/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#version 460

out vec4 out_color;

in vec3 v_normal;

void main() {
    vec3 light_dir = vec3(0.0, 0.0, -1.0);

    vec3 n = normalize(v_normal);
    float diffuse = max(dot(n, normalize(-light_dir)), 0.0);
    out_color = vec4(vec3(1.0, 0.3, 0.7) * diffuse, 1.0);
}