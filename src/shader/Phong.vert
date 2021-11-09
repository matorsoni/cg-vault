#version 400 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex;

out vec3 P;
out vec3 N;
out vec3 L;
out vec3 light_space_pos;

// Transforms and geometry data.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_light_view;
uniform mat4 u_light_projection;
uniform vec4 u_clip_plane;

uniform vec3 u_light_position;

void main()
{
    mat4 model_view = u_view * u_model;

    // Transform to view coordinates.
    // Vertex position.
    vec4 P4 = model_view * vec4(in_pos, 1.0);
    P = vec3(P4) / P4.w;
    // Vertex normal.
    N = normalize(
       transpose(inverse(mat3(model_view))) * in_normal
    );
    // Backwards light direction.
    vec4 light4 = u_view * vec4(u_light_position, 1.0);
    vec3 light3 = vec3(light4) / light4.w;
    L = normalize(light3 - P);

    gl_Position = u_projection * P4;
    vec4 light_space_pos4 = u_light_projection * u_light_view * P4;
    light_space_pos = light_space_pos4.xyz / light_space_pos4.w;
    // Note: position is in view coords while u_clip_plane is in world coords.
    gl_ClipDistance[0] = dot(P4, u_clip_plane);
}
