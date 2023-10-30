#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
//layout (location = 2) in vec2 aTex;
//not using texture coordinates until after reading week xD

uniform mat4 u_transform;
out vec3 color;
out vec3 normal;

void main()
{
   // Convert our normals from [-1, 1] to [0, 1]
   normal = aNorm;
   color = vec3(1.0);//aNorm * 0.5 + 0.5;
   gl_Position = u_transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
