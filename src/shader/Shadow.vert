#version 400 core

layout (location = 0) in vec3 in_pos;

// Transforms and geometry data.
uniform mat4 u_model;
uniform mat4 u_light_view;
uniform mat4 u_light_projection;

void main()
{
    gl_Position = u_light_projection * u_light_view * u_model * vec4(in_pos, 1.0);
}
