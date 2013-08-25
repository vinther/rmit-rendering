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

subroutine vec3 textureOutputType(vec2 uv);
subroutine uniform textureOutputType textureOutput;

uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler;

layout(location = 0) out vec4 RT0;

in vec2 texCoord; 

uniform float test;
uniform vec3 pingStart;

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
 


uniform int ambientOcclusionOnly;
uniform int enableAmbientOcclusion;
uniform int enableLighting;

void main()
{
    vec4 DS = texture2D(DSSampler, texCoord);
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

    //if (1 == ambientOcclusionOnly)
    //    fragColor = vec4(1.f) * ao;
    //else
    //    fragColor = diffuse * ao;

    RT0 += RT3;
    RT0.w = 1.0f;
}	