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
in vec3 vertTexCoord;

out vec4 fragColor;

uniform sampler2D texSampler;
//uniform sampler2D ambientTexSampler;
uniform sampler2D diffuseTexSampler;
//uniform sampler2D specularTexSampler;

void main()
{
    mat4 worldToCameraMatrix = (view * model);

    vec3 lightPos = normalize(vec3(1.0, 1.0, 0.0));
    vec3 cameraPosition = -worldToCameraMatrix[3].xyz * mat3(worldToCameraMatrix);
    
    const vec3 lightColor = vec3(1, 1, 1);
    const vec3 globalAmbient = vec3(0.2, 0.2, 0.2);

  // Position in eye space
    vec3 P = vec3(vertPosition);

  // Normalize normal in eye space
    vec3 N = normalize(vertNormal);

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
    float specularLight = pow(max(dot(N, H),0), 256.0);
    if(diffuseLight <= 0)
    	specularLight = 0;
    vec3 specular = globalAmbient * lightColor * specularLight;

  // Define the final vertex color
    gl_FragColor.xyz = emissive + ambient + diffuse + specular;
    //gl_FragColor.xyz = specular;
    //gl_FragColor = lightSourceParameters[0].position;
    //gl_FragColor = lightSourceParameters[0].ambient;
    //gl_FragColor = frontMaterial.emission;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}