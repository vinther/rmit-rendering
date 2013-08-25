#version 410

layout(location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

smooth out vec4 fragPosition;
flat out vec4 fragLightPosition;

void main()
{
	fragPosition = viewMatrix * modelMatrix * position;
	fragLightPosition = viewMatrix * modelMatrix * vec4(0.0, 0.0, 0.0, 1.0);
	
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * position;
}