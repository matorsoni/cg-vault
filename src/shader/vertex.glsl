#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;

out vec4 vertex_color;
out vec2 vertex_tex;

void main()
{
   gl_Position = vec4(position, 1.0);
   vertex_color = vec4(color, 1.0);
   vertex_tex = tex_coord;
}
