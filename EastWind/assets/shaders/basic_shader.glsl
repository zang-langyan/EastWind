#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_VPMatrix;

out vec3 v_Position;
out vec4 v_Color;

void main()
{
  v_Position = aPos;
  vec3 normal_color = ( aNormal + 1.f ) * 0.5f;
  v_Color = vec4(normal_color,1.f);
  gl_Position = u_VPMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
    FragColor = v_Color;
    // FragColor = vec4(v_Position*0.5f+0.5f, 1.0f);
}
