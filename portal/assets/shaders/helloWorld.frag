#version 330

in vec2 uv;
in vec4 DEBUG_COLOR;

out vec4 color;

uniform sampler2D texSampler;

void main()
{
    color = texture(texSampler, uv) * DEBUG_COLOR;
}