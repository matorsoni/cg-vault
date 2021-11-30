#version 400 core

in vec3 P;
in vec3 N;
in vec3 L;
in vec4 light_space_pos;
in vec2 tex_coords;

out vec4 FragColor;

// Phong material.
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shiny;

// Lighting.
uniform float u_ambient_coef;
uniform float u_diffuse_coef;
uniform float u_specular_coef;

// Shadow map texture.
uniform sampler2D shadow_map;
// Regular texture.
uniform sampler2D object_texture;

const vec3 light_color = vec3(1.0, 1.0, 1.0);
const float depth_bias = 0.0001;

float computeShadow(vec4 frag_light_space_pos)
{
    // Perform perspective divide and transform to [0,1] range.
    vec3 projCoords = (frag_light_space_pos.xyz / frag_light_space_pos.w) * 0.5 + 0.5;
    // Get closest depth value from the shadow map.
    float closest_depth = texture(shadow_map, projCoords.xy).x;
    // Get depth of current fragment from light's perspective.
    float current_depth = projCoords.z;
    // Check whether current frag position is in shadow.
    return current_depth > closest_depth + depth_bias ? 1.0 : 0.0;
}

void main()
{
    vec3 normal = normalize(N);
    vec3 light  = normalize(L);

    vec4 tex_color = texture(object_texture, tex_coords);

    // Lighting components.
    vec3 ambient = u_ambient_coef * u_ka * vec3(tex_color);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    float incidence = dot(light, normal);
    if (incidence >= 0.0) {
        diffuse = u_diffuse_coef * incidence * u_kd * vec3(tex_color);

        // Reflected light vector.
        vec3 R = reflect(-light, normal);
        // Vector to viewer.
        vec3 V = -normalize(P);
        float specAngle = max(dot(R, V), 0.0);
        specular = u_specular_coef * pow(specAngle, u_shiny) * u_ks;
    }
    float shadow = computeShadow(light_space_pos);
    vec3 final_color = (ambient + (1.0 - shadow) * (diffuse + specular)) * light_color;
    FragColor = vec4(final_color, 1.0);
}
