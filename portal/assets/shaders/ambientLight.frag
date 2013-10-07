#version 410

uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler;

layout(location = 0) out vec4 RT0;

in vec2 texCoord; 

uniform mat4 viewProjectionInverse;
uniform vec3 cameraPosition;

// http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
vec3 decode(vec2 enc)
{
    vec2 fenc = enc * 4.0f - 2.0f;
    float f = dot(fenc, fenc);
    float g = sqrt(1.0f - f / 4.0f);
    vec3 n = vec3(fenc * g, 1.0f - f / 2.0f);
    return n;
}

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
vec3 viewSpacePos(vec2 uv, float zOverW)
{ 
    vec4 H = vec4(uv * 2.0f - 1.0f, zOverW * 2.0f - 1.0f, 1.0f);  
    vec4 D = viewProjectionInverse * H;

    return vec3(D / D.w);  
}

// http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
float LinearizeDepth(float zOverW, float near, float far)
{
  return (2.0 * near) / (far + near - zOverW * (far - near)); 
}

const float distanceThreshold = 100.0f;
const vec2 filterRadius = vec2(0.02f);
const uint sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
                                vec2( -0.94201624,  -0.39906216 ),
                                vec2(  0.94558609,  -0.76890725 ),
                                vec2( -0.094184101, -0.92938870 ),
                                vec2(  0.34495938,   0.29387760 ),
                                vec2( -0.91588581,   0.45771432 ),
                                vec2( -0.81544232,  -0.87912464 ),
                                vec2( -0.38277543,   0.27676845 ),
                                vec2(  0.97484398,   0.75648379 ),
                                vec2(  0.44323325,  -0.97511554 ),
                                vec2(  0.53742981,  -0.47373420 ),
                                vec2( -0.26496911,  -0.41893023 ),
                                vec2(  0.79197514,   0.19090188 ),
                                vec2( -0.24188840,   0.99706507 ),
                                vec2( -0.81409955,   0.91437590 ),
                                vec2(  0.19984126,   0.78641367 ),
                                vec2(  0.14383161,  -0.14100790 )
                               );

float SSAO(vec3 viewPos, vec3 viewNormal, vec2 uv, float depth)
{
    float ambientOcclusion = 0;
    // perform AO
    for (uint i = 0; i < sample_count; ++i)
    {
        // sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
        vec2 sampleTexCoord = uv + (poisson16[i] * (filterRadius));
        float sampleDepth = texture(DSSampler, sampleTexCoord).r;
        vec3 samplePos = viewSpacePos(sampleTexCoord, sampleDepth);
        vec3 sampleDir = normalize(samplePos - viewPos);
 
        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max(dot(viewNormal, sampleDir), 0);
        // distance between SURFACE-POSITION and SAMPLE-POSITION
        float VPdistSP = distance(viewPos, samplePos);
 
        // a = distance function
        float a = 1.0 - smoothstep(distanceThreshold, distanceThreshold * 2, VPdistSP);
        // b = dot-Product
        float b = NdotS;
 
        ambientOcclusion += (a * b);
    }
 
    return 1.0f - (ambientOcclusion / sample_count);
}

uniform int enableSSAO;

void main()
{
    vec4 DS = texture2D(DSSampler, texCoord);
    vec4 RT1 = texture2D(RT1Sampler, texCoord);
    vec4 RT3 = texture2D(RT3Sampler, texCoord);
    
    float zOverW = DS.x;
    vec2 packedNormal = RT1.rg;
    
    vec3 N = decode(packedNormal);
    vec3 P = viewSpacePos(texCoord, zOverW);
    float depth = LinearizeDepth(zOverW, 1.0f, 4000.0f);
    
    float ao = 1.0f;
    
    if (1 == enableSSAO)
    	ao = SSAO(P, N, texCoord, depth);
    
    RT0 = 0.5f * RT3 * ao;
    RT0.w = 1.0f;
}	