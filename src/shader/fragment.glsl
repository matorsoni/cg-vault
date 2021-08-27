#version 400 core

in vec2 v_tex;

out vec4 f_color;

uniform sampler2D u_sampler;

void main()
{
    f_color = texture(u_sampler, v_tex);
}
