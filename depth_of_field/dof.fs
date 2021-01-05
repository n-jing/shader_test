#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec2 screen_size;

void main()
{
  vec4 origin_color = texture2D(texture1, TexCoord);

  vec3 blur_color = vec3(0.0f, 0.0f, 0.0f);
  
  int kernel_num = 12;
  vec2 kernel_base[12];
  kernel_base[0] = vec2(1.0,0.0);
  kernel_base[1] = vec2(0.5,0.866);
  kernel_base[2] = vec2(-0.5,0.866);
  kernel_base[3] = vec2(-1.0,0.0);
  kernel_base[4] = vec2(-0.5,-0.866);
  kernel_base[5] = vec2(0.5,-0.866);
  kernel_base[6] = vec2(1.5,0.866);
  kernel_base[7] = vec2(0, 1.732);
  kernel_base[8] = vec2(-1.5,0.866);
  kernel_base[9] = vec2(-1.5,0.866);
  kernel_base[10] = vec2(0,-1.732);
  kernel_base[11] = vec2(1.5,-0.866);

  float coc = origin_color.a * 3.0f;
  for (int i = 0; i < kernel_num; ++i)
  {
    vec2 off_set = vec2(kernel_base[i].x * screen_size.x * coc,
                        kernel_base[i].y * screen_size.y * coc);
    vec3 off_color = texture2D(texture1, TexCoord + off_set).rgb;
    blur_color += off_color;
  }

  FragColor = vec4(blur_color / kernel_num, 1.0f);
  /* FragColor = origin_color; */
} 
