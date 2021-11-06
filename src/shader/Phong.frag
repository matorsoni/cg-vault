#version 400 core

in vec3 P;
in vec3 N;
in vec3 L;
in vec4 light_space_pos;

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

const vec3 light_color = vec3(1.0, 1.0, 1.0);

float computeShadow(vec4 frag_light_space_pos)
{
    // perform perspective divide
    vec3 projCoords = frag_light_space_pos.xyz / frag_light_space_pos.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    return currentDepth > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec3 normal = normalize(N);
    vec3 light  = normalize(L);

    // Lighting components.
    vec3 ambient = u_ambient_coef * u_ka;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    float incidence = dot(light, normal);
    if (incidence >= 0.0) {
        diffuse = u_diffuse_coef * incidence * u_kd;

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
