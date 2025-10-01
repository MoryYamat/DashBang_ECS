// ModelData (Universal Data Types)
#pragma once

#include <vector>
#include <string>

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

#define MAX_BONE_INFLUENCE 4

namespace Engine::Graphics::Model
{
	struct TextureData
	{
		GLuint id = 0;
		std::string type;// "diffuse", "specular", "normal", etc.
		std::string path;


		TextureData() = default;
		~TextureData()
		{
			if (id != 0)
			{
				glDeleteTextures(1, &id);
			}
		}

		TextureData(const TextureData&) = delete;
		TextureData& operator= (const TextureData&) = delete;

		TextureData(TextureData&& o) noexcept
			: id(std::exchange(o.id, 0)), type(std::move(o.type)), path(std::move(o.path)) { }


		TextureData& operator=(TextureData&& o) noexcept {
			if (this != &o) {
				if (id) glDeleteTextures(1, &id);
				id = std::exchange(o.id, 0);
				type = std::move(o.type);
				path = std::move(o.path);
			}
			return *this;
		}
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

		MeshGPU() = default;
		~MeshGPU() {
			if (ebo) glDeleteBuffers(1, &ebo);
			if (vbo) glDeleteBuffers(1, &vbo);
			if (vao) glDeleteVertexArrays(1, &vao);
		}

		MeshGPU(const MeshGPU&) = delete;
		MeshGPU& operator=(const MeshGPU&) = delete;

		MeshGPU(MeshGPU&& o) noexcept {
			*this = std::move(o);
		}
		MeshGPU& operator=(MeshGPU&& o) noexcept {
			if (this != &o) {
				// 先に自分の分を片付け
				if (ebo) glDeleteBuffers(1, &ebo);
				if (vbo) glDeleteBuffers(1, &vbo);
				if (vao) glDeleteVertexArrays(1, &vao);

				vao = std::exchange(o.vao, 0);
				vbo = std::exchange(o.vbo, 0);
				ebo = std::exchange(o.ebo, 0);
				indexCount = o.indexCount;
			}
			return *this;
		}
	};

	struct ModelGPU
	{
		std::vector<MeshGPU> meshesGPU;
	};
}

