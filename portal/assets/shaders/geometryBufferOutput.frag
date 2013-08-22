#version 410

uniform sampler2D RT0Sampler;
uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler; 

subroutine vec3 textureOutputType(vec2 uv);
subroutine uniform textureOutputType textureOutput;

in vec2 texCoord; 
out vec4 fragColor;

uniform mat4 viewProjectionInverse;

// http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
vec3 decode (vec2 enc)
{
    vec2 fenc = enc * 4.0f - 2.0f;
    float f = dot(fenc, fenc);
    float g = sqrt(1.0f - f / 4.0f);
    vec3 n = vec3(fenc * g, 1.0f - f / 2.0f);
    return n;
}

// http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
float LinearizeDepth(float zOverW, float near, float far)
{
  return (2.0 * near) / (far + near - zOverW * (far - near)); 
}

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
vec3 viewSpacePos(vec2 uv, float zOverW)
{ 
    vec4 H = vec4(uv * 2.0f - 1.0f, zOverW, 1.0f);  
    vec4 D = viewProjectionInverse * H;

    return vec3(D / D.w);  
}

subroutine(textureOutputType)

vec3 depth(vec2 uv)
{
    vec4 DS = texture2D(DSSampler, uv);
    float depth = LinearizeDepth(DS.x, 1.0f, 4000.0f);

    return vec3(depth, depth, depth);
}

subroutine(textureOutputType)

vec3 albedo(vec2 uv)
{
    vec4 RT3 = texture2D(RT3Sampler, uv);

    return RT3.xyz;
}

subroutine(textureOutputType)

vec3 normals(vec2 uv)
{
    vec4 RT1 = texture2D(RT1Sampler, uv);
    vec2 packedNormal = RT1.rg;
        
    vec3 N = decode(packedNormal);

    return decode(packedNormal);
}

subroutine(textureOutputType)

vec3 positions(vec2 uv)
{
    vec4 DS = texture2D(DSSampler, uv);

    return viewSpacePos(uv, DS.x);
}

void main()
{
    fragColor.xyz = textureOutput(texCoord);
    fragColor.w = 1.0f;
}   