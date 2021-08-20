#version 330 core

in vec4 vertex_color;
out vec4 frag_color;

uniform vec4 input_color;

void main()
{
    frag_color = vertex_color + input_color;
}
