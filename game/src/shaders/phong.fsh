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
in vec2 v_uv;

uniform vec3 u_view_pos;


/*
    Phong surface material.
*/
struct PhongMaterial {
    sampler2D diffuse; // Color the surface reflects under diffuse (direct) and ambient lighting.
    vec3 emissive; // Color the surface emits (self-illumination).
    sampler2D specular; // Color of the specular highlight on the surface.
    float shininess; // Intensity of the specular highlight.
};

uniform PhongMaterial material;


/*
    Basic point light.
*/
struct PointLight {
    vec3 position; // Position of light source in world space.
    vec3 color; // Color of the light.
    float ambient_intensity; // Ambient intensity of the light.
};

#define N_POINT_LIGHTS 8
uniform PointLight point_lights[N_POINT_LIGHTS];
uniform int point_lights_count;


/*
    Calculate point light radiance at given frag position.
*/
vec3 point_light_radiance(
    PointLight light,
    vec3 normal,
    vec2 uv,
    vec3 frag_pos,
    vec3 view_dir
) {
    float light_constant = 1.0;
    float light_linear = 0.09;
    float light_quadratic = 0.032;

    vec3 light_delta = light.position - frag_pos;
    vec3 light_dir = normalize(light_delta);
    float light_dist = length(light_delta);

    // Fatt = 1 / (Kc + Kl * d + Kq * d^2)
    float attenuation = 1.0 / (light_constant + light_linear * light_dist + light_quadratic * (light_dist * light_dist));

    vec3 diffuse_sample = texture(material.diffuse, uv).rgb;
    vec3 specular_sample = texture(material.specular, uv).rgb;

    // Ambient lighting
    vec3 ambient = diffuse_sample * (light.color * light.ambient_intensity);

    // Diffuse lighting
    float diffuse_strength = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = (diffuse_strength * diffuse_sample) * light.color;

    // Specular lighting
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_strength = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = (specular_strength * specular_sample) * light.color;

    return attenuation * (ambient + diffuse + specular) + material.emissive;
}


void main() {
    vec2 uv = vec2(1.0 - v_uv.x, 1.0 - v_uv.y);
    vec3 view_dir = normalize(u_view_pos - v_frag_pos);

    vec3 radiance = vec3(0.0);

    for (int i = 0; i < point_lights_count; i++) {
        radiance += point_light_radiance(
            point_lights[i],
            v_normal,
            uv,
            v_frag_pos,
            view_dir
        );
    }

    out_color = vec4(radiance, 1.0);
}