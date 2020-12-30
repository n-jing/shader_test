#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

out float v_depth;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
void main()
{
  vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
  vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

  gl_Position = view * model * vec4(aPos, 1.0);
  v_depth = gl_Position.z;
  gl_Position = projection * gl_Position;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
