// ModelData (Universal Data Types)
#pragma once

#include <vector>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>


// CPU-side data
struct VertexData
{
	glm::vec3 position;
};

struct MeshData
{
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	bool hasIndices = false;
};

struct ModelData
{
	std::vector<MeshData> meshes;

};

// GPU-side data
struct MeshGPU
{
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	unsigned int indexCount = 0;
};

struct ModelGPU
{
	std::vector<MeshGPU> meshesGPU;
};