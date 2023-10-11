#include "Mesh.h"
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

#include <Math.h>
#include <vector>

void CreateMesh(Mesh& mesh, const char* path)
{
	std::vector<Vector3> out_vertices;
	std::vector<Vector2> out_uvs;
	std::vector<Vector3> out_normals;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	FILE* file = fopen(path, "r");
	//fopen_s(&file, path, "r");

	if (!file)
	{
		printf("MeshCache: could not open file: %s", path);
	}

	while (true)
	{
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)	//break if the result of the file operation indicates we've reached the end
			break;
		if (strcmp(lineHeader, "v") == 0) //if the first part of the line before spaces is equivalent to "v" we are dealing with a vertex:
		{
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)	//if the first part of the line before spaces is equivalent to "vt" we are dealing with a texture coordinate:
		{
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1.0f - uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)	//if the first part of the line before spaces is equivalent to "vn" we are dealing a vertex normal:
		{
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) //if the first part of the line before spaces is equivalent to "vn" we are dealing with a face:
		{
			//std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			for (int i = 0; i < 3; i++) {
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
	}

	mesh.vertexCount = vertexIndices.size();
	out_vertices.resize(mesh.vertexCount);
	out_normals.resize(mesh.vertexCount);
	out_uvs.resize(mesh.vertexCount);

	// For each vertex of each triangle
	for (unsigned int i = 0; i < mesh.vertexCount; i++) {
		out_vertices[i] = temp_vertices[vertexIndices[i] - 1];
		out_normals[i] = temp_normals[normalIndices[i] - 1];
		out_uvs[i] = temp_uvs[uvIndices[i] - 1];
	}

	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.positions);
	glGenBuffers(1, &mesh.normals);
	glGenBuffers(1, &mesh.uvs);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.positions);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(Vector3), out_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.normals);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(Vector3), out_normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvs);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(Vector2), out_uvs.data(), GL_STATIC_DRAW);

	glBindVertexArray(mesh.vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.positions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, mesh.normals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvs);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)(0));
}
