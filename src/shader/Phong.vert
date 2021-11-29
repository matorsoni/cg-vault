#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

out vec3 P;
out vec3 N;
out vec3 L;
out vec4 light_space_pos;

// Transforms and geometry data.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_light_view;
uniform mat4 u_light_projection;

uniform vec3 u_light_position;

void main()
{
    // Transform vertex position and normal to view coordinates.
    vec4 world_pos = u_model * vec4(in_pos, 1.0);
    vec4 view_pos = u_view * world_pos;
    P = vec3(view_pos) / view_pos.w;
    // Vertex normal.
    N = normalize(
       transpose(inverse(mat3(u_view * u_model))) * in_normal
    );

    // Backwards light direction.
    vec4 light4 = u_view * vec4(u_light_position, 1.0);
    vec3 light3 = vec3(light4) / light4.w;
    L = normalize(light3 - P);

    gl_Position = u_projection * view_pos;
    light_space_pos = u_light_projection * u_light_view * world_pos;
}
