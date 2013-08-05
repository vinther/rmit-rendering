#version 330

struct MaterialParameters  
{   
   vec4 emission;   
   vec4 ambient; 
   vec4 diffuse; 
   vec4 specular; 
   float shininess; 
};

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

layout(std140) uniform MaterialParametersLoc {
	MaterialParameters frontMaterial;
	MaterialParameters backMaterial;
};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 vertNormal;
in vec3 vertPosition;
in vec3 vertTangent;
in vec3 vertTexCoord;

out vec4 fragColor;

uniform sampler2D ambientTexSampler;
uniform sampler2D diffuseTexSampler;
uniform sampler2D specularTexSampler;
uniform sampler2D normalTexSampler;
uniform sampler2D bumpTexSampler;

uniform float test;

void main()
{
    mat4 worldToCameraMatrix = (view * model);

    vec3 cameraPosition = -worldToCameraMatrix[3].xyz * mat3(worldToCameraMatrix);
    vec3 lightPos = vec3(sin(test) * 16, -11.0 + cos(test) * 4 + 4, 0.0);
    
    const vec3 lightColor = vec3(1, 1, 1);
    const vec3 globalAmbient = vec3(0.2, 0.2, 0.2);
    
    vec3 binormal = cross(vertTangent, vertNormal);
  	mat3 rotation = mat3(vertTangent, binormal, vertNormal);
  		
  	/* http://stackoverflow.com/a/5284527 */
	const ivec3 off = ivec3(-1, 0, 1);
	const vec2 size = vec2(1.0, 0.0);  		
	vec4 wave = texture(bumpTexSampler, vertTexCoord.xy);
    float s11 = wave.x;
    float s01 = textureOffset(bumpTexSampler, vertTexCoord.xy, off.xy).x;
    float s21 = textureOffset(bumpTexSampler, vertTexCoord.xy, off.zy).x;
    float s10 = textureOffset(bumpTexSampler, vertTexCoord.xy, off.yx).x;
    float s12 = textureOffset(bumpTexSampler, vertTexCoord.xy, off.yz).x;
    vec3 va = normalize(vec3(size.xy, s21 - s01));
    vec3 vb = normalize(vec3(size.yx, s12 - s10));
    vec3 bump = cross(va,vb);
  
  // Position in eye space
    vec3 P = vec3(mat3(model) * vertPosition);

  // Normalize normal in eye space
  	vec3 N;
  	
  	if (length(vertTangent) > 0.1f)
  		N = normalize((rotation * bump) + vertNormal);
  	else
  		N = normalize(vertNormal);

  // Compute the emissive term
    vec3 emissive = vec3(frontMaterial.emission);

  // Compute the ambient term
    vec3 ambient = vec3(frontMaterial.ambient) * globalAmbient;

  // Compute the diffuse term
  // Normalized vector toward the light source
    vec3 L = normalize(vec3(lightPos) - P);
    float diffuseLight = max(dot(N, L), 0);
    vec3 diffuse = vec3(frontMaterial.diffuse) * lightColor * diffuseLight * vec3(texture(diffuseTexSampler, vec2(vertTexCoord)));

  // Compute the specular term
    vec3 V = normalize(-P);      // Normalized vector toward the viewpoint
    vec3 H = normalize(L + V);   // Normalized vector that is halfway between V and L
    float specularLight = pow(max(dot(N, H),0), frontMaterial.shininess);
    if(diffuseLight <= 0)
    	specularLight = 0;
    vec3 specular = vec3(frontMaterial.specular) * lightColor * specularLight;

  	// Define the final fragment color
    fragColor.xyz =  emissive + ambient + diffuse + specular;
}