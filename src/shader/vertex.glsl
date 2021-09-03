#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

uniform mat4 u_view_projection;
uniform mat4 u_model;

out vec3 v_normal;
out vec2 v_tex;

void main()
{
   gl_Position = u_view_projection * u_model * vec4(in_pos, 1.0);
   v_normal = in_normal;
   v_tex = in_tex;
}
