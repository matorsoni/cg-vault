#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_tex;

// Outputs of vertex shader.
out vec4 v_color;
out vec2 v_tex;

uniform mat4 u_mat;

void main()
{
   gl_Position = u_mat * vec4(in_pos, 1.0);
   v_color = vec4(in_color, 1.0);
   v_tex = in_tex;
}
