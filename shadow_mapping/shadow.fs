#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec3 FragPos;


uniform sampler2D shadowMap;
uniform sampler2D texture_diffuse1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  if (projCoords.z > 1.0f || projCoords.z < 0.0f)
    return 0.0f;
  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(shadowMap, projCoords.xy).r; 
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // check whether current frag pos is in shadow
  float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

  return shadow;
}


void main()
{    
  vec3 color = texture(texture_diffuse1, TexCoords).rgb;
  vec3 normal = normalize(Normal);
  vec3 ambient = 0.3 * color;
  vec3 lightDir = normalize(lightPos - FragPos);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;
  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  spec = pow(max(dot(normal, halfwayDir), 0.0), 6.0);
  vec3 specular = spec * lightColor;    
  // calculate shadow
  float shadow = ShadowCalculation(FragPosLightSpace, bias);                      
  /* vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;     */
  vec3 lighting = (ambient + (1.0f-shadow)*(diffuse + specular)) * color;
    
  FragColor = vec4(lighting, 1.0);
  /* FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); */

}
