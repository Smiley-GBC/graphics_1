#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;
uniform float u_time;

void main()
{
   color = aCol;
   gl_Position = vec4(aPos.x + sin(u_time), aPos.y, aPos.z, 1.0);
}
