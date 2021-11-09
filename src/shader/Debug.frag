#version 400 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadow_map;

void main()
{
    float depthValue = texture(shadow_map, TexCoords).x;
    FragColor = vec4(vec3(depthValue), 1.0);
    //FragColor = vec4(TexCoords, 0.0, 1.0);
}
