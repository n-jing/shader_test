#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
