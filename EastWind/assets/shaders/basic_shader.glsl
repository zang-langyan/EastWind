#type vertex
#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTangent;
layout (location = 3) in vec4 vColor;


uniform mat4 u_ModelMatrix;
uniform mat4 u_VPMatrix;

out vec3 v_Position;
out vec4 v_Color;
out vec3 v_Normal;
out vec3 v_WordPos;

out mat3 tbnMatrix;

void main()
{
  v_Position = vPos;
  v_Normal = vNormal;
  vec3 normal_color = ( vNormal + 1.f ) * 0.5f;
  v_Color = vec4(normal_color,1.f);
  // v_Color = vColor;
  v_WordPos = vec3(u_ModelMatrix * vec4(vPos, 1));
  gl_Position = u_VPMatrix * u_ModelMatrix * vec4(vPos.x, vPos.y, vPos.z, 1.0);

  vec3 bitangent = cross(vTangent, vNormal);
	vec3 T = normalize(vec3(u_ModelMatrix * vec4(vTangent, 0.0)));
	vec3 B = normalize(vec3(u_ModelMatrix * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(u_ModelMatrix * vec4(vNormal, 0.0)));
  tbnMatrix = mat3(T, B, N);
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec3 v_Position;
in vec3 v_WordPos;
in vec3 v_Normal;
in vec4 v_Color;
in mat3 tbnMatrix;

// struct Material{
//     vec3 ka;
//     vec3 kd;
//     vec3 ks;
//     float ns;
//     vec3 color;
//     float illumination;
// } material;
// material.ka = vec3(1);
// material.kd = vec3(1);
// material.ks = vec3(1);

// struct Light {
//     vec3 position;
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     vec3 radiance;
// } light;
// light.position = vec3(0., 0., 20.);
// light.ambient = vec3(0., 0., 20.);
// light.diffuse = vec3(0., 0., 20.);
// light.specular = vec3(0., 0., 20.);

void main()
{
  // Phong Illumination
  // vec3 diffuseColor =vec3(0);
  // vec3 ambientColor =vec3(0);
  // vec3 specularColor =vec3(0);

  // vec3 lightDir = normalize(light.position - v_WordPos);
  // float diffDot = max(dot(v_Normal, lightDir), 0.0);
  // diffuseColor += diffDot * light.radiance;
  // float ambientFactor = 0.1f;
  // ambientColor += (light.ambient.xyz * mat.ka);

  // float spec;
  // if (diffDot>0){
  //     vec3 viewDir = vec3(normalize(camPos - worldPos));
  //     vec3 reflectDir = reflect(-lightDir, normal);
  //     float specDot = max(dot(viewDir, reflectDir), 0.0);
  //     spec = pow(specDot, 32);
  // } else {
  //     spec = 0;
  // }
  // float specStrength = 1.0;
  // specularColor += specStrength * spec * lights[i].radiance;


  FragColor = v_Color;
  // FragColor = vec4(v_Position*0.5f+0.5f, 1.0f);
  // FragColor = vec4(
  //             (ambientColor + diffuseColor) * colorMap.rgb 
  //             + (specularColor * specValue) 
  //             + mat.illumination * mat.color, 
  //             1.0
  //           );
}
