#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;
uniform float u_time;
uniform float u_time2;

void main()
{
   color = aCol;
   gl_Position = vec4(aPos.x + cos(u_time2), aPos.y + sin(u_time2), aPos.z, 1.0);
}
