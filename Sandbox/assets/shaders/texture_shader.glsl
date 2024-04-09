#type vertex
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_VPMatrix;
uniform mat4 u_ModelMatrix;

out vec2 v_TexCoord;

void main()
{
  v_TexCoord = aTexCoord;
  gl_Position = u_ModelMatrix * u_VPMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#type fragment
#version 410 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;  

void main()
{
  FragColor = texture(u_Texture, v_TexCoord);
  // FragColor = vec4(v_TexCoord.x, v_TexCoord.y, 1.f, 1.0f);
}
