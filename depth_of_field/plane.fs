#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec4 FragPosLightSpace;
} fs_in;


in vec2 TexCoord;
in float v_depth;

// texture sampler
uniform sampler2D shadowMap;
uniform sampler2D texture1;

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
  vec3 color = texture(texture1, TexCoord).rgb;
  vec3 normal = normalize(fs_in.Normal);
  vec3 lightColor = lightColor;
  // ambient
  vec3 ambient = 0.3 * color;
  // diffuse
  vec3 lightDir = normalize(lightPos - fs_in.FragPos);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = 0.5 * diff * lightColor;
  // specular
  vec3 viewDir = normalize(viewPos - fs_in.FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  spec = pow(max(dot(normal, halfwayDir), 0.0), 12.0);
  vec3 specular = spec * lightColor;    
  // calculate shadow
  float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);                      
  /* vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;     */
  vec3 lighting = (ambient + (1.0f-shadow)*(diffuse + specular)) * color;
    


  float blur = 0;
  float near_distance = 3.0;
  float far_distance = 3.0;
  float near_plane = 3.0;
  float far_plane = 4.0;

  if(v_depth <= near_plane && v_depth >= far_plane)
  {
    blur = 0;
  }
  else if(v_depth > near_plane)
  {
    blur = clamp(v_depth, near_plane, near_plane + near_distance);
    blur = (blur - near_plane) / near_distance;
  }
  else if(v_depth < far_plane)
  {
    blur = clamp(v_depth, far_plane - far_distance, far_plane);
    blur = (far_plane - blur) / far_distance;
  }

  FragColor = vec4(lighting, 1.0f);
  /* FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); */
}
