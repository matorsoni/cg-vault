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
   mat4 model_view = u_view * u_model;

   // Transform to view coordinates.
   // Vertex position.
   vec4 P4 = model_view * vec4(in_pos, 1.0);
   vec3 P3 = vec3(P4) / P4.w;
   // Vertex normal.
   vec3 N = normalize(
      transpose(inverse(mat3(model_view))) * in_normal
   );
   // Backwards light direction.
   vec3 L = -normalize(mat3(u_view) * u_light_direction);

   // Lighting components.
   vec3 ambient = u_ambient_coef * u_ka;
   vec3 diffuse = vec3(0.0);
   vec3 specular = vec3(0.0);

   float incidence = dot(L, N);
   if (incidence >= 0.0) {
      diffuse = u_diffuse_coef * incidence * u_kd;

      // Reflected light vector.
      vec3 R = reflect(-L, N);
      // Vector to viewer.
      vec3 V = -normalize(P3);
      float specAngle = max(dot(R, V), 0.0);
      specular = u_specular_coef * pow(specAngle, u_shiny) * u_ks;
   }
   // Final vertex color:
   vec3 final_color = (ambient + diffuse + specular) * light_color;

   // Outputs.
   gl_Position = u_projection * P4;
   // Note: position is in view coords while u_clip_plane is in world coords.
   gl_ClipDistance[0] = dot(P4, u_clip_plane);
   v_color = vec4(final_color, 1.0);
   v_tex = in_tex;
}
