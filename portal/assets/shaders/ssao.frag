#version 410

uniform sampler2D RT1Sampler;
uniform sampler2D DSSampler;

layout(location = 0) out vec4 RT0;

in vec2 texCoord; 

uniform mat4 viewProjectionInverse;
uniform vec3 cameraPosition;





void main()
{
    vec4 DS = texture2D(DSSampler, texCoord);
    vec4 RT1 = texture2D(RT1Sampler, texCoord);
    
    float zOverW = DS.x;
    vec2 packedNormal = RT1.rg;
    
    vec3 N = decode(packedNormal);
    vec3 P = viewSpacePos(texCoord, zOverW);
    float depth = LinearizeDepth(zOverW, 1.0f, 4000.0f);
    
    float ao = SSAO(P, N, texCoord, depth);
    
    RT0.xyz = vec3(ao);
    //RT0.w = 1.0f;
}	
