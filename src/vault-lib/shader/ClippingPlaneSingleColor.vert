#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

uniform mat4 u_view_projection;
uniform mat4 u_model;
uniform vec4 u_clip_plane;
uniform vec3 u_color;

out vec4 v_color;
out vec2 v_tex;


void main()
{
   vec4 world_pos = u_model * vec4(in_pos, 1.0);
   gl_Position = u_view_projection * world_pos;
   gl_ClipDistance[0] = dot(world_pos, u_clip_plane);
   v_color = vec4(u_color, 1.0f);
   v_tex = in_tex;
}
