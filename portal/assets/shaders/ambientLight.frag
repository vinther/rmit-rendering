#version 410

uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler;

layout(location = 0) out vec4 RT0;

in vec2 texCoord; 

uniform mat4 viewProjectionInverse;
uniform vec3 cameraPosition;

void main()
{
    vec4 RT3 = texture2D(RT3Sampler, texCoord);

    RT0 = 0.5f * RT3;
    RT0.w = 1.0f;
}	