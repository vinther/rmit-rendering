#version 410

in vec4 position;

out vec2 texCoord;

void main(void)
{
   gl_Position = position * 2.0f - 1.0f;
   gl_Position.z = 0.0;
   
   texCoord = position.xy;
}