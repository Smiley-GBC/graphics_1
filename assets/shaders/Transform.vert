#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform mat4 u_transform;
out vec3 color;

void main()
{
   // Convert our normals from [-1, 1] to [0, 1]
   color = aCol * 0.5 + 0.5;
   //color = aCol;
   gl_Position = u_transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
