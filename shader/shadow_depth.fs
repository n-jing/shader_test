#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D shadowMap;

void main()
{           
  FragColor = vec4(texture(shadowMap, TexCoord).r);
}
