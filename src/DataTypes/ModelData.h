// ModelData (Universal Data Types)
#pragma once

#include <vector>
#include <string>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct TextureData
{
	unsigned int id;
	std::string type;// "diffuse", "specular", "normal", etc.
	std::string path;
};

struct MaterialData
{
	std::vector<TextureData> textures;
	glm::vec3 baseColor = glm::vec3(1.0f);
};

// CPU-side data
struct VertexData
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	int boneIDs[MAX_BONE_INFLUENCE] = { 0 };
	float weights[MAX_BONE_INFLUENCE] = { 0 };
};

struct MeshData
{
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	bool hasIndices = false;

	MaterialData materialData;
};

// 
struct ModelData
{
	std::vector<MeshData> meshes;

	// model's size datas
	glm::vec3 min = glm::vec3(FLT_MAX);
	glm::vec3 max = glm::vec3(-FLT_MAX);

	glm::vec3 GetSize() const { return max - min; }
	glm::vec3 GetCenter() const { return (min + max) * 0.5f; }

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