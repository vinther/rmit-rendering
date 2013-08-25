#version 410

struct MaterialParameters  
{   
   vec4 emission;   
   vec4 ambient; 
   vec4 diffuse; 
   vec4 specular; 
   float shininess; 
};

layout(std140) uniform MaterialParametersLoc {
    MaterialParameters frontMaterial;
    MaterialParameters backMaterial;
};

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

smooth in vec3 vertexNormal;
smooth in vec3 vertexNormalWorld;
smooth in vec3 vertexPosition;
smooth in vec3 vertexTangent;
smooth in vec2 vertexTexCoord;

//uniform sampler2D emissiveTexSampler;
//uniform sampler2D ambientTexSampler;
uniform sampler2D diffuseTexSampler;
//uniform sampler2D specularTexSampler;
uniform sampler2D bumpTexSampler;

uniform bool enableBumpMapping;

layout(location = 0) out vec4 RT1;
layout(location = 1) out vec4 RT2;
layout(location = 2) out vec4 RT3;

// http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
vec2 encode (vec3 n)
{
    float p = sqrt(max(n.z * 8.0f + 8.0f, 0.0f));
    return vec2(n.xy/p + 0.5f);
}

//Returns a normal from a grid of heights

vec3 findNormal(vec2 uv, int u)
{   
    //u is one uint size, ie 1.0/texture size
    ivec2 offsets[4];
    offsets[0] = ivec2(-u, 0);
    offsets[1] = ivec2(u, 0);
    offsets[2] = ivec2(0, -u);
    offsets[3] = ivec2(0, u);

    float hts[4];
    for(int i = 0; i < 4; i++)
    {
        hts[i] = 2 * textureOffset(bumpTexSampler, uv, offsets[i]).x;
    }

    vec2 _step = vec2(1.0, 0.0);

    vec3 va = normalize(vec3(_step.xy, hts[1]-hts[0]));
    vec3 vb = normalize(vec3(_step.yx, hts[3]-hts[2]));

    return cross(va,vb); //you may not need to swizzle the normal
}

void main()
{
    const float Epsilon = 0.0001f;

    vec3 binormal = cross(vertexTangent, vertexNormal);
    mat3 rotation = mat3(vertexTangent, binormal, vertexNormal);

    vec3 normal;
    if (enableBumpMapping && length(texture2D(bumpTexSampler, vertexTexCoord)) > 0.01f)
    {
        normal = rotation * findNormal(vertexTexCoord, 1);// + vertexNormal;   
    } else
    {
        normal = vertexNormal;
    }
     
    vec3 transformedNormal = vec3(normal.xy, max(normal.z, -1.0 + Epsilon));
    
    RT1.zw = vec2(1.0f, 1.0f);
    RT1.xy = encode(normalize(transformedNormal));
    RT2 = vec4(0.0f, 0.0f, 0.0f, frontMaterial.shininess) + frontMaterial.diffuse;
    RT3 = texture(diffuseTexSampler, vertexTexCoord);
}   