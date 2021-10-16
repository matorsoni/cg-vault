#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_clip_plane;
uniform vec3 u_color;
uniform vec3 u_light_direction;

out vec4 v_color;
out vec2 v_tex;

// Ambient illumination strength.
const float k_a = 0.2;
const float k_d = 1.0;
const vec3 light_color = vec3(1.0, 1.0, 1.0);


void main()
{
   vec4 world_pos = u_model * vec4(in_pos, 1.0);
   vec3 normal = normalize(
      transpose(inverse(mat3(u_model))) * in_normal
   );
   vec3 light_direction = normalize(u_light_direction);

   gl_Position = u_projection * u_view * world_pos;
   gl_ClipDistance[0] = dot(world_pos, u_clip_plane);

   // 1) Ambient light.
   vec3 ambient = k_a * light_color;
   // 2) Diffuse light.
   vec3 diffuse = k_d * max(0.0, dot(-light_direction, normal)) * light_color;
   // Final vertex color:
   vec3 final_color = (ambient + diffuse) * u_color;
   v_color = vec4(final_color, 1.0);

   v_tex = in_tex;
}
