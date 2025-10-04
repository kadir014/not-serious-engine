/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#version 460

out vec4 out_color;

in vec3 v_normal;
in vec3 v_frag_pos;

uniform vec3 u_view_pos;

void main() {
    vec3 light_pos = vec3(0.0, 10.0, 10.0);
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 albedo = vec3(1.0, 0.3, 0.7);

    vec3 light_dir = normalize(light_pos - v_frag_pos);

    // Ambient lighting
    float ambient_intensity = 0.1;
    vec3 ambient = ambient_intensity * light_color;

    // Diffuse lighting
    float diffuse_strength = max(dot(v_normal, light_dir), 0.0);
    vec3 diffuse = diffuse_strength * light_color;

    // Specular lighting
    int specular_power = 64;
    float specular_intensity = 0.7;
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, v_normal);
    float specular_strength = pow(max(dot(view_dir, reflect_dir), 0.0), specular_power);
    vec3 specular = specular_strength * light_color * specular_intensity;

    vec3 final_color = (ambient + diffuse + specular) * albedo;
    out_color = vec4(final_color, 1.0);
}