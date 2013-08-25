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


smooth in vec4 fragPosition;
flat in vec4 fragLightPosition;

uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler;

layout(location = 0) out vec4 RT0;

uniform float test;
uniform vec3 pingStart;

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
    int screenWidth = 1280;
    int screenHeight = 720;

    float radius = 200;
    vec3 diffuseLightColor = vec3(1.0f, 0.0f, 1.0f);

	vec2 texCoord = (gl_FragCoord).xy;
	texCoord.x = texCoord.x / screenWidth;
	texCoord.y = texCoord.y / screenHeight;

    vec4 DS = texture2D(DSSampler, texCoord);
    vec4 RT1 = texture2D(RT1Sampler, texCoord);
    vec4 RT3 = texture2D(RT3Sampler, texCoord);

    float zOverW = DS.x;
    vec2 packedNormal = RT1.rg;
    vec4 albedo = RT3;
        
    vec3 N = decode(packedNormal);
    vec3 P = viewSpacePos(texCoord, zOverW);
	
    //Lighting Calcs (view space)
   	vec3 ltop = fragLightPosition.xyz - P;
   	float diffuseModifier = max(dot(N.xyz,normalize(ltop)), 0.0);
    float noZTestFix = step(0.0, radius-length(ltop)); //0.0 if dist > radius, 1.0 otherwise
    float attenuation = 1 / ((((length(ltop)/(1-((length(ltop)/radius)*(length(ltop)/radius))))/radius)+1)*(((length(ltop)/(1-((length(ltop)/radius)*(length(ltop)/radius))))/radius)+1));
    attenuation = 0.5f;

    vec4 diffuse = diffuseModifier * vec4(diffuseLightColor, 1.0) * attenuation * noZTestFix;

    //RT0.xyz = normalize(fragPosition.xyz);
    RT0 = diffuseModifier * vec4(diffuseLightColor, 1.0);
    //RT0.xyz = fragLightPosition.xyz;
    RT0 = diffuse;
    RT0.w = 0.5f;
}	