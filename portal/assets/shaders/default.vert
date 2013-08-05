#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out	vec3 vertNormal;
out	vec3 vertPosition;
out	vec3 vertTangent;
out	vec3 vertTexCoord;

void main()
{
	vertNormal = normal;
	vertPosition = position;
	vertTexCoord = texCoord;
    vertTangent = tangent;
    
    gl_Position = projection * view * model * vec4(position, 1.0f);
}