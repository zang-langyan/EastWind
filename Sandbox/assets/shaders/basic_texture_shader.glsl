#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_VPMatrix;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
  v_Position = aPos;
  v_Color = vec4(aNormal,1.f);
  v_TexCoord = aTexCoord;
  gl_Position = u_VPMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;  

void main()
{
  FragColor = texture(u_Texture, v_TexCoord);

  FragColor = vec4(.1f, .9f, .2f, .8f);
}
