#version 400 core

in vec3 P;
in vec3 N;
in vec3 L;

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

const vec3 light_color = vec3(1.0, 1.0, 1.0);

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
    vec3 final_color = (ambient + diffuse + specular) * light_color;
    FragColor = vec4(final_color, 1.0);
}
