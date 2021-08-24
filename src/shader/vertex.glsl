#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

// Outputs of vertex shader.
out vec2 v_tex;

uniform mat4 u_mat;
uniform mat4 u_model;

void main()
{
   gl_Position = u_mat * u_model * vec4(in_pos, 1.0);
   v_tex = in_tex;
}
