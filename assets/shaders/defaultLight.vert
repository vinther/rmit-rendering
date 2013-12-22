#version 410

layout(location = 0) in vec4 vertPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

smooth out vec4 fragPosition;
smooth out vec4 fragLightPosition;

uniform vec4 position;
uniform vec4 color;
uniform float radius;

void main()
{
	fragPosition = viewMatrix * modelMatrix * vertPos;
	fragLightPosition = position;
	
	gl_Position = projectionMatrix * viewMatrix * ((mat4(mat3(1.0f) * radius)) * modelMatrix * vertPos + position);
}