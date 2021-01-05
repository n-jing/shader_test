#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float v_depth;

uniform sampler2D texture_diffuse1;

void main()
{    
  FragColor = texture(texture_diffuse1, TexCoords);
  float blur = 0;

  float dis = 3.0;

  float near = -10.0;
  float far = -9.0;

  if(v_depth >= near && v_depth <= far)
  {
    blur = 0;
  }
  else if(v_depth < near)
  {
    blur = clamp(abs(v_depth - near) / dis, 0.0f, 1.0f);
  }
  else if(v_depth > far)
  {
    blur = clamp((v_depth-far) / dis, 0.0f, 1.0f);
  }
  else
  {
    blur = 0.0f;
  }
  
  FragColor.a = blur;
}
