#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

uniform mat4 u_mat;
uniform mat4 u_model;

out vec4 v_color;
out vec2 v_tex;

vec3 color_from_normal(vec3 normal)
{
   return normalize(abs(normal));
}

void main()
{
   gl_Position = u_mat * u_model * vec4(in_pos, 1.0);
   v_color = vec4(color_from_normal(in_normal), 1.0f);
   v_tex = in_tex;
}
