#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexure;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normal;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main()
{
   position = (u_model * vec4(aPosition, 1.0)).xyz;
   normal = u_normal * aNormal;
   uv = aTexure;

   gl_Position = u_mvp * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
