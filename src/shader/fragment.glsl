#version 330 core

in vec2 v_tex;

out vec4 f_color;

uniform vec4 u_color;
uniform sampler2D u_sampler0;
uniform sampler2D u_sampler1;

void main()
{
    f_color = mix(texture(u_sampler0, v_tex), texture(u_sampler1, v_tex), 0.5) + 0.2 * u_color;
}
