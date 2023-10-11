#pragma once
#include <glad/glad.h>

struct Mesh
{
	size_t vertexCount = 0;
	GLuint vao;

	// Must store handles to vertex buffers so we don't leak memory
	GLuint positions;
	GLuint uvs;
	GLuint normals;
};

void CreateMesh(Mesh& mesh, const char* path);
void DestroyMesh(Mesh& mesh);