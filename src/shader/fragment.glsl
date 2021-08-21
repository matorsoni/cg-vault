#version 330 core

in vec4 v_color;
in vec2 v_tex;

// Output of fragment shader.
out vec4 f_color;

uniform vec4 u_color;
uniform sampler2D u_sampler0;
uniform sampler2D u_sampler1;

void main()
{
    f_color = mix(texture(u_sampler0, v_tex), texture(u_sampler1, v_tex), 0.5) * (v_color + u_color);
}
