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
vec3 decodeNormal (vec2 enc)
{
    vec2 fenc = enc * 4.0f - 2.0f;
    float f = dot(fenc, fenc);
    float g = sqrt(1.0f - f / 4.0f);
    vec3 n = vec3(fenc * g, 1.0f - f / 2.0f);
    return n;
}

// http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
float linearizeDepth(float zOverW, float near, float far)
{
  return (2.0 * near) / (far + near - zOverW * (far - near)); 
}

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
vec3 viewSpacePos(vec2 uv, float zOverW)
{ 
    vec4 H = vec4(uv * 2.0f - 1.0f, 2.0 * zOverW - 1.0, 1.0f);  
    vec4 D = viewProjectionInverse * H;

    return (D / D.w).xyz;  
}

subroutine(textureOutputType)

vec3 depth(vec2 uv)
{
    vec4 DS = texture2D(DSSampler, uv);
    return vec3(linearizeDepth(DS.x, 1.0f, 4000.0f));
}

subroutine(textureOutputType)

vec3 lightAccumulation(vec2 uv)
{
    return texture2D(RT0Sampler, uv).xyz;
}

subroutine(textureOutputType)

vec3 albedo(vec2 uv)
{
    return texture2D(RT3Sampler, uv).xyz;
}

subroutine(textureOutputType)

vec3 normals(vec2 uv)
{
    return decodeNormal(texture2D(RT1Sampler, uv).rg);
}

subroutine(textureOutputType)

vec3 positions(vec2 uv)
{
    return viewSpacePos(uv, texture2D(DSSampler, uv).x);
}

void main()
{
    fragColor.xyz = textureOutput(texCoord);
    fragColor.w = 1.0f;
}   