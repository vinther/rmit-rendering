#version 410

layout(location = 0) out vec4 RT0;

smooth in vec4 fragPosition;
smooth in vec4 fragLightPosition;

uniform sampler2D RT1Sampler;
uniform sampler2D RT2Sampler;
uniform sampler2D RT3Sampler;
uniform sampler2D DSSampler;

uniform float test;
uniform vec3 pingStart;

uniform mat4 viewProjectionInverse;
uniform vec3 cameraPosition;

uniform vec4 position;
uniform vec4 color;
uniform float radius;

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
    vec4 H = vec4(uv * 2.0f - 1.0f, zOverW * 2.0f - 1.0f, 1.0f);  
    vec4 D = viewProjectionInverse * H;

    return vec3(D / D.w);  
}

void main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

	vec2 texCoord = (gl_FragCoord).xy;
	texCoord.x = texCoord.x / screenWidth;
	texCoord.y = texCoord.y / screenHeight;

    vec4 DS = texture2D(DSSampler, texCoord);
    vec4 RT1 = texture2D(RT1Sampler, texCoord);
    vec4 RT3 = texture2D(RT3Sampler, texCoord);

    float zOverW = DS.x;
    vec2 packedNormal = RT1.rg;
    vec4 albedo = RT3;
        
    vec3 N = normalize(decode(packedNormal));
    vec3 P = viewSpacePos(texCoord, zOverW);
	
    //Lighting Calcs (view space)
    
    vec3 L = fragLightPosition.xyz - P;
   	float diffuseModifier = max(dot(N,normalize(L)), 0.0);
    float noZTestFix = step(0.0, radius-length(L)); //0.0 if dist > radius, 1.0 otherwise
    
    float cutoff = 0.9f;
    float distance = length(L);
    // calculate basic attenuation
    float denom = max(distance, 0) / radius + 1;
    float attenuation = 1 / (denom * denom);
     
    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);
	attenuation = 1.0f - smoothstep(0.0, radius, distance);

    RT0 = color * albedo * noZTestFix * attenuation * diffuseModifier;
}	