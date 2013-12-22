#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 texCoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

smooth out vec3 vertexNormal;
smooth out vec3 vertexPosition;
smooth out vec3 vertexTangent;
smooth out vec2 vertexTexCoord;

void main()
{
	vertexPosition = position;
	vertexNormal = normalize(vec3(normalMatrix * vec4(normal, 1.0f)));
	vertexNormal = normalize(vec3(vec4(normal, 1.0f)));
	vertexTangent = tangent;
	vertexTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	
	gl_Position =  projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}