#version 410

layout(location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

smooth out vec4 fragPosition;
smooth out vec4 fragLightPosition;

flat out vec4 color;
flat out float radius;

struct LightData
{
	vec4 position;
	vec4 color;
	float radius, X, Y, Z;
};

layout(std140) uniform LightDataLoc
{
	LightData lights[1024];
};

void main()
{
	fragPosition = viewMatrix * modelMatrix * position;
	fragLightPosition = lights[gl_InstanceID].position;
	
	radius = lights[gl_InstanceID].radius;
	color = lights[gl_InstanceID].color;
	
	gl_Position = projectionMatrix * viewMatrix * ((mat4(mat3(1.0f) * radius)) * modelMatrix * position + lights[gl_InstanceID].position);
}