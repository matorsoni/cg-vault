#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

// Transforms and geometry data.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec4 u_clip_plane;

// Phong material.
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shiny;

// Lighting.
uniform vec3 u_light_direction;
uniform float u_ambient_coef;
uniform float u_diffuse_coef;
uniform float u_specular_coef;

out vec4 v_color;
out vec2 v_tex;

const vec3 light_color = vec3(1.0, 1.0, 1.0);


void main()
{
   vec4 world_pos = u_model * vec4(in_pos, 1.0);
   vec3 normal = normalize(
      transpose(inverse(mat3(u_model))) * in_normal
   );
   vec3 L = normalize(-u_light_direction);

   gl_Position = u_projection * u_view * world_pos;
   gl_ClipDistance[0] = dot(world_pos, u_clip_plane);

   // 1) Ambient light.
   vec3 ambient = u_ambient_coef * u_ka;
   // 2) Diffuse light.
   vec3 diffuse = u_diffuse_coef * max(0.0, dot(L, normal)) * u_kd;
   // 3) Specular light.
   vec3 R = reflect(-L, normal);      // Reflected light vector
   vec3 V = normalize(-vec3(u_view * world_pos)); // Vector to viewer
   float specAngle = max(dot(R, V), 0.0);
   vec3 specular = u_specular_coef * pow(specAngle, u_shiny) * u_ks;
   // Final vertex color:
   vec3 final_color = ambient + diffuse + specular;
   v_color = vec4(final_color, 1.0);

   v_tex = in_tex;
}
