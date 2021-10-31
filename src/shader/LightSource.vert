#version 400 core

layout (location = 0) in vec3 in_pos;

// Transforms and geometry data.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_color;

const vec3 light_color = vec3(1.0, 1.0, 1.0);

void main()
{
   // Outputs.
   gl_Position = u_projection * u_view * u_model * vec4(in_pos, 1.0);
   v_color = vec4(light_color, 1.0);
}
