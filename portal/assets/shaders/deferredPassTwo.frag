#version 410

 struct LightSourceParameters 
{   
   vec4 ambient;   
   vec4 diffuse;   
   vec4 specular;   
   vec4 position;
}; 

layout(std140) uniform LightSourceParametersLoc {
	LightSourceParameters lightSourceParameters[8];
};

uniform sampler2D RT0Sampler;
uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler; 

in vec2 texCoord; 
out vec4 fragColor;

uniform float test;
uniform vec3 pingStart;//

/*
    //vec3 cameraPosition = -worldToCameraMatrix[3].xyz * mat3(worldToCameraMatrix);
    vec3 lightPos = vec3(modelViewMatrix * vec4(sin(test) * 1000, 200, 0.0, 0.0));
    
    const vec4 lightColor = vec4(1, 1, 1, 1.0);
    const vec4 globalAmbient = vec4(0.2, 0.2, 0.2, 1.0);

  // Compute the emissive term
    vec4 emissive = frontMaterial.emission;

  // Compute the ambient term
    vec4 ambient = frontMaterial.ambient * globalAmbient * texture(ambientTexSampler, vertexTexCoord);

  // Compute the diffuse term
  // Normalized vector toward the light source
    vec3 L = normalize(vec3(lightPos) - P);
    float diffuseLight = max(dot(N, L), 0);
    vec4 diffuse = frontMaterial.diffuse * lightColor * diffuseLight * texture(diffuseTexSampler, vertexTexCoord);

  // Compute the specular term
    vec3 V = normalize(-P);      // Normalized vector toward the viewpoint
    vec3 H = normalize(L + V);   // Normalized vector that is halfway between V and L
    float specularLight = pow(max(dot(N, H),0), frontMaterial.shininess);
    if(diffuseLight <= 0)
    	specularLight = 0;
    vec4 specular = frontMaterial.specular * lightColor * specularLight;
    */

uniform mat4 viewProjectionInverse;
uniform vec3 cameraPosition;

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
 
    //return 1.0f;
    return 1.0f - (ambientOcclusion / sample_count);
}

uniform int ambientOcclusionOnly;
uniform int enableAmbientOcclusion;
uniform int enableLighting;

void main()
{
    vec4 DS = texture2D(DSSampler, texCoord);
    vec4 RT0 = texture2D(RT0Sampler, texCoord);
    vec4 RT1 = texture2D(RT1Sampler, texCoord);
    vec4 RT2 = texture2D(RT2Sampler, texCoord);
    vec4 RT3 = texture2D(RT3Sampler, texCoord);

    float zOverW = DS.x;
    vec2 packedNormal = RT1.rg;
    vec4 albedo = RT3;

    float depth = LinearizeDepth(zOverW, 1.0f, 4000.0f);
    float specularIntensity = RT2.a;
		
    vec3 N = decode(packedNormal);
    vec3 P = viewSpacePos(texCoord, zOverW);

    vec3 lightPos = vec3(vec4(sin(test * 4.0f) * 1400.0f, 200, 0.0, 0.0));
    
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);



    

  // Compute the diffuse term
  // Normalized vector toward the light source

    vec3 L = normalize(vec3(lightPos) - P);
    float diffuseLight;

    if (1 == enableLighting)
        diffuseLight = max(dot(N, L), 0);
    else
        diffuseLight = 1.0f;

    vec4 diffuse = lightColor * diffuseLight * albedo;

  // Compute the specular term
    vec3 V = normalize(-P);      // Normalized vector toward the viewpoint
    vec3 H = normalize(L + V);   // Normalized vector that is halfway between V and L
    float specularLight = pow(max(dot(N, H),0), specularIntensity);
    if(diffuseLight <= 0)
      specularLight = 0;

    if (specularIntensity < 0.001f)
        specularLight = 0.0f;

    vec4 specular = lightColor * specularLight;

    float t = mod(test * 1000.0f, 2000.0f);
    float d = depth * 4000.0f;

    vec4 col;

    if (t - d > 8.0f && t - d < 128.0f)    
        col = vec4(0.0, 0.8, 0.8, 0.0f) * (1.0 - clamp(log(t-d) / log(128.0), 0.0, 1.0)) * (1.0 - smoothstep(0.0f, 2000.0, d));

    float ao = 1.0f;
    
    if (1 == enableAmbientOcclusion)
        ao = SSAO(P, N, texCoord, depth);

    if (1 == ambientOcclusionOnly)
        fragColor = vec4(1.f) * ao;
    else
        fragColor = diffuse * ao;

    //fragColor = lightColor * diffuseLight;

    fragColor.w = 1.0f;
}	