#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
} fs_in;


in vec2 TexCoord;
in float v_depth;

// texture sampler
uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

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
  /* vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;     */
  vec3 lighting = (ambient + (diffuse + specular)) * color;



  float blur = 0;
  float dis = 40.0;
  float near = -12.0;
  float far = -4.0;

  if(v_depth >= near && v_depth <= far)
    blur = 0;
  else if(v_depth < near)
  {
    float no = abs(v_depth - near) / dis;
    blur = clamp(no*no, 0.0f, 1.0f);
  }
    
  else
    blur = 0.0f;

  FragColor = vec4(lighting, blur);
  /* FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); */
}
