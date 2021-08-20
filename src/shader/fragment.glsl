#version 330 core

in vec4 vertex_color;
in vec2 vertex_tex;

out vec4 frag_color;

uniform vec4 input_color;
uniform sampler2D input_tex;

void main()
{
    frag_color = texture(input_tex, vertex_tex)+ 0.2*(vertex_color + input_color);
}
