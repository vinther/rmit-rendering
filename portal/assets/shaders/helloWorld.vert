#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 uv;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    uv = texCoord.xy;
}