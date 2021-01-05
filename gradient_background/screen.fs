#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec2 screen_size;

void main()
{
  vec4 origin_color = texture2D(texture1, TexCoord);

  vec3 color = mix(vec3(origin_color), vec3(1.0f, 1.0f, 1.0f), origin_color.a);
  FragColor = vec4(color, 1.0f);
} 
